/*
 * Copyright (c) 2015-2018, Luca Fulchir<luker@fenrirproject.org>,
 * All rights reserved.
 *
 * This file is part of "libRaptorQ".
 *
 * libRaptorQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * libRaptorQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * and a copy of the GNU Lesser General Public License
 * along with libRaptorQ.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "RaptorQ/RaptorQ_v1_hdr.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <stdlib.h>
#include <vector>
#include "schifra_utilities.hpp"

#define VLC_VERBOSE_OUTPUT 0
#define SYMBOL_SIZE 6
#define OVERHEAD_SIZE 10

// rename the main namespace for ease of use
namespace RaptorQ = RaptorQ__v1;

RaptorQ::Block_Size globalBlockSize;
schifra::utils::timer timer;

// input data
std::string orignal_mes = "SCUNO1MsA7yYslHg2fXYO0oolrtsYQc8VeRF0JxFE8Zwrhk47KaJQ1ZnRKzmEC54PcRoTpmQWWPo1urzixZdvlmYtMnTx1mPWwShSD9pJ2HDGgBjC8yeJyqd8QPrUwl6";
std::vector<uint8_t> global_input_data;



/**
 * Generate encoded symbols.
 * @param mysize The original data size in bytes.
 * @param rand The random engine that used to generate random data.
 * @param symbols_size N bytes in a symbol, which will be transmitted through VLC physical frame.
 * @param overhead N repair symbols
 */

std::vector<std::pair<uint8_t, std::vector<uint8_t>>>
generate_symbols(const uint32_t mysize, std::mt19937_64 &rnd,
                 const uint16_t symbol_size, const uint16_t overhead) {

    // The input vector.
    std::vector<uint8_t> input(mysize);

    // Fill it with random data
    std::uniform_int_distribution<int16_t> distr(0,
                                                 std::numeric_limits<uint8_t>::max());
    // fill our input with random data
    std::copy_n(global_input_data.begin(),mysize,input.begin());
#if VLC_VERBOSE_OUTPUT
    std::cout << "===== Input data =====" << std::endl;
//    for (size_t idx = 0; idx < mysize; ++idx) {
//        uint8_t iiidata = static_cast<uint8_t> (distr(rnd));
//        printf("%d,", iiidata);
//        input.push_back(iiidata);
//    }
    for(auto i:input){
        printf("%d,",i);
    }
    std::cout << std::endl << "===== Input data =====\n" << std::endl;
#endif

    // how many symbols do we need to encode all our input in a single block?
    auto min_symbols = (input.size() * sizeof(uint8_t)) / symbol_size;
    if ((input.size() * sizeof(uint8_t)) % symbol_size != 0)
        ++min_symbols;
    // convert "symbols" to a typesafe equivalent, RaptorQ::Block_Size
    // This is needed becouse not all numbers are valid block sizes, and this
    // helps you choose the right block size
    RaptorQ::Block_Size block = RaptorQ::Block_Size::Block_10;
    for (auto blk: *RaptorQ::blocks) {
        // RaptorQ::blocks is a pointer to an array, just scan it to find your
        // block.
        if (static_cast<uint16_t> (blk) >= min_symbols) {
            block = blk;
            globalBlockSize = blk;
            break;
        }
    }

    // now initialize the encoder.
    // the input for the encoder is std::vector<uint8_t>
    // the output for the encoder is std::vector<uint8_t>
    // yes, you can have different types, but most of the time you will
    // want to work with uint8_t
    RaptorQ::Encoder<typename std::vector<uint8_t>::iterator,
            typename std::vector<uint8_t>::iterator> enc(
            block, symbol_size);

    // give the input to the encoder. the encoder answers with the size of what
    // it can use
    if (enc.set_data(input.begin(), input.end()) != mysize) {
        std::cout << "Could not give data to the encoder :(\n";
        exit(1);
    }

    // actual symbols. you could just use static_cast<uint16_t> (blok)
    // but this way you can actually query the encoder.
    uint16_t _symbols = enc.symbols();
#if VLC_VERBOSE_OUTPUT
        std::cout << "===== Encoder Parameters =====" << std::endl;

        std::cout << "Block Size: " << static_cast<uint16_t>(block) << std::endl;
        // print some stuff in output
        std::cout << "Data Size: " << mysize << std::endl <<
                  "Symbol Nums: " << static_cast<uint32_t> (_symbols) << std::endl <<
                  "Symbol Size: " << static_cast<int32_t>(enc.symbol_size()) << "\n";

        std::cout << "===== Encoder Parameters =====\n" << std::endl;
#endif
    // RQ need to do its magic on the input before you can ask the symbols.
    // multiple ways to do this are available.
    // The simplest is to run the computation and block everything until
    // the work has been done. Not a problem for small sizes (<200),
    // but big sizes will take **a lot** of time, consider running this with the
    // asynchronous calls
    if (!enc.compute_sync()) {
        // if this happens it's a bug in the library.
        // the **Decoder** can fail, but the **Encoder** can never fail.
        std::cout << "Enc-RaptorQ failure! really bad!\n";
        exit(1);
    }

    // we will store here all encoded and transmitted symbols
    // std::pair<symbol id (esi), symbol data>
    using symbol_id = uint8_t; // just a better name
    std::vector<std::pair<symbol_id, std::vector<uint8_t>>> received;
    uint32_t received_tot = 0;


    // Now get the source symbols.
    // source symbols are specials because they contain the input data
    // as-is, so if you get all of these, you don't need repair symbols
    // to make sure that we are using the decoder, drop the first
    // source symbol.
    auto source_sym_it = enc.begin_source();
    for (; source_sym_it != enc.end_source(); ++source_sym_it) {
        // we save the symbol here:
        // make sure the vector has enough space for the symbol:
        // fill it with zeros for the size of the symbol
        std::vector<uint8_t> source_sym_data(symbol_size, 0);

        auto it = source_sym_data.begin();
        auto written = (*source_sym_it)(it, source_sym_data.end());
        if (written != symbol_size) {
            // this can only happen if "source_sym_data" did not have
            // enough space for a symbol (here: never)
            std::cout << written << "-vs-" << symbol_size <<
                      " Could not get the whole source symbol!\n";
            exit(1);
        }

        // good, the symbol was received.
        ++received_tot;
        // add it to the vector of received symbols
        symbol_id tmp_id = (*source_sym_it).id();
        received.emplace_back(tmp_id, std::move(source_sym_data));

    }

    //--------------------------------------------
    // we finished working with the source symbols.
    // now we need to transmit the repair symbols.
    auto repair_sym_it = enc.begin_repair();
    auto max_repair = enc.max_repair();

    timer.start();
    for (uint32_t i = 0; i < overhead && repair_sym_it != enc.end_repair(max_repair); ++repair_sym_it, ++i) {
        std::vector<uint8_t> repair_sym_data(symbol_size, 0);
        auto it = repair_sym_data.begin();
        auto written = (*repair_sym_it)(it, repair_sym_data.end());
        if (written != symbol_size) {
            // this can only happen if "repair_sym_data" did not have
            // enough space for a symbol (here: never)
            std::cout << written << "-vs-" << symbol_size <<
                      " Could not get the whole repair symbol!\n";
            exit(1);
        }

        // good, the symbol was received.
        ++received_tot;
        // add it to the vector of received symbols
        symbol_id tmp_id = (*repair_sym_it).id();
        received.emplace_back(tmp_id, std::move(repair_sym_data));
    }
    timer.stop();
    std::cout<<"Raptor encode time:"<<timer.time()<<"encode_time_end"<<std::endl;

#if VLC_VERBOSE_OUTPUT
    std::cout << "===== Encoded Symbols =====" << std::endl;
//    std::cout << "Symbol ID \t Symbol\n";
    for (auto p: received) {
        printf("%d,", p.first);
        for (auto s: p.second) {
            printf("%d,", s);
        }
//        std::cout << std::endl;
    }
    std::cout << "===== Encoded Symbols =====\n" << std::endl;
#endif
    return received;
}

/**
 * Decode received symbols.
 * @param received The received symbols and they are assumed to be not error.
 * @param mysize The original data size.
 * @param symbol_size N bytes in a symbol.
 */
void decode_symbols(std::vector<std::pair<uint8_t, std::vector<uint8_t>>> received,
                    const uint32_t mysize, const uint16_t symbol_size) {
    using symbol_id = uint32_t;

// define "Decoder_type" to write less afterwards
    using Decoder_type = RaptorQ::Decoder<
            typename std::vector<uint8_t>::iterator,
            typename std::vector<uint8_t>::iterator>;
    Decoder_type dec(globalBlockSize, symbol_size, Decoder_type::Report::COMPLETE);
// "Decoder_type::Report::COMPLETE" means that the decoder will not
// give us any output until we have decoded all the data.
// there are modes to extract the data symbol by symbol in an ordered
// an unordered fashion, but let's keep this simple.


// we will store the output of the decoder here:
// note: the output need to have at least "mysize" bytes, and
// we fill it with zeros
    std::vector<uint8_t> output(mysize, 0);

    // now push every received symbol into the decoder
#if VLC_VERBOSE_OUTPUT
        std::cout << "===== Received Symbols =====" << std::endl;
        std::cout << "Symbol ID \t Symbol \n";
#endif
    for (auto &rec_sym: received) {
        // as a reminder:
        //  rec_sym.first = symbol_id (uint32_t)
        //  rec_sym.second = std::vector<uint8_t> symbol_data
        symbol_id tmp_id = rec_sym.first;
#if VLC_VERBOSE_OUTPUT
            std::cout << tmp_id << "\t";
#endif
        auto it = rec_sym.second.begin();
        auto err = dec.add_symbol(it, rec_sym.second.end(), tmp_id);
        if (err != RaptorQ::Error::NONE && err != RaptorQ::Error::NOT_NEEDED) {
            // When you add a symbol, you can get:
            //   NONE: no error
            //   NOT_NEEDED: libRaptorQ ignored it because everything is
            //              already decoded
            //   INITIALIZATION: wrong parameters to the decoder contructor
            //   WRONG_INPUT: not enough data on the symbol?
            //   some_other_error: errors in the library
            std::cout << "error adding?\n";
            exit(1);
        }
#if  VLC_VERBOSE_OUTPUT
            for (uint8_t data: rec_sym.second) {
                printf("%d ", data);
            }
            std::cout << std::endl;
#endif
    }
#if VLC_VERBOSE_OUTPUT
        std::cout << "===== Received Symbols =====\n" << std::endl;
#endif

    // by now we now there will be no more input, so we tell this to the
    // decoder. You can skip this call, but if the decoder does not have
    // enough data it sill wait forever (or until you call .stop())
    dec.end_of_input(RaptorQ::Fill_With_Zeros::NO);
    // optional if you want partial decoding without using the repair
    // symbols
    // std::vector<bool> symbols_bitmask = dec.end_of_input (
    //                                          RaptorQ::Fill_With_Zeros::YES);

    // decode, and do not return until the computation is finished.
    timer.start();
    auto res = dec.wait_sync();
    if (res.error != RaptorQ::Error::NONE) {
        std::cout << "Couldn't decode.\n";
        timer.stop();
        std::cout<<"Raptor decode time:"<<timer.time()<<"decode_time_end"<<std::endl;
        exit(1);
    }
    timer.stop();
    std::cout<<"Raptor decode time:"<<timer.time()<<"decode_time_end"<<std::endl;

    // now save the decoded data in our output
    size_t decode_from_byte = 0;
    size_t skip_bytes_at_begining_of_output = 0;
    auto out_it = output.begin();
    auto decoded = dec.decode_bytes(out_it, output.end(), decode_from_byte,
                                    skip_bytes_at_begining_of_output);
    // "decode_from_byte" can be used to have only a part of the output.
    // it can be used in advanced setups where you ask only a part
    // of the block at a time.
    // "skip_bytes_at_begining_of_output" is used when dealing with containers
    // which size does not align with the output. For really advanced usage only
    // Both should be zero for most setups.


    if (decoded.written != mysize) {
        if (decoded.written == 0) {
            // we were really unlucky and the RQ algorithm needed
            // more symbols!
            std::cout << "Couldn't decode, RaptorQ Algorithm failure. "
                         "Can't Retry.\n";
        } else {
            // probably a library error
            std::cout << "Partial Decoding? This should not have happened: " <<
                      decoded.written << " vs " << mysize << "\n";
        }
        exit(1);
    } else {
//        std::cout << "Decoded: " << mysize << "\n";
    }
    // byte-wise check: did we actually decode everything the right way?
    for (uint64_t i = 0; i < mysize; ++i) {
        if(output[i]!=global_input_data[i])
        {
            std::cout<<"decode failed"<<std::endl;
            exit(1);
        }
    }
}



int main(int argc, char **argv) {
    // Get a random number generator and fixed its seed.
    std::mt19937_64 rnd;
    uint64_t seed = 114514;
    rnd.seed(seed);

    for(auto ch:orignal_mes){
        global_input_data.push_back(ch);
    }
    int mysize = orignal_mes.size();
    // Generate encoded symbols
    std::vector<std::pair<uint8_t, std::vector<uint8_t>>>
            symbols = generate_symbols(mysize, rnd, SYMBOL_SIZE, OVERHEAD_SIZE);



    std::vector<std::pair<uint8_t, std::vector<uint8_t>>>
            received_symbols;
    // Get the real input from the VLC link
    for (int i = 1; i< argc; i+=SYMBOL_SIZE+1)
    {
        uint8_t id = atoi(argv[i]);
        std::vector<uint8_t> temp_symbols;
        for(int j = i + 1;j<i+1+SYMBOL_SIZE;j++)
        {
            temp_symbols.push_back(atoi(argv[j]));
        }
        received_symbols.emplace_back(id,std::move(temp_symbols));

    }
    // Raptor Decode
    decode_symbols(received_symbols, mysize, SYMBOL_SIZE);

    return 0;
}
