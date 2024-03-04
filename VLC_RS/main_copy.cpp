#include <cstddef>
#include <iostream>
#include <string>
#include <random>
#include <vector>

#include "schifra_galois_field.hpp"
#include "schifra_galois_field_polynomial.hpp"
#include "schifra_sequential_root_generator_polynomial_creator.hpp"
#include "schifra_reed_solomon_encoder.hpp"
#include "schifra_reed_solomon_decoder.hpp"
#include "schifra_reed_solomon_block.hpp"
#include "schifra_error_processes.hpp"


/* Finite Field Parameters */
const std::size_t field_descriptor                =   8;
const std::size_t generator_polynomial_index      = 120;
const std::size_t generator_polynomial_root_count =  32;

/* Instantiate Finite Field and Generator Polynomials */
const schifra::galois::field field(field_descriptor,
                                   schifra::galois::primitive_polynomial_size06,
                                   schifra::galois::primitive_polynomial06);

schifra::galois::field_polynomial generator_polynomial(field);

/**
 * Check the filed configuration, math stuff, didn't care.
 */
void check_field(){
    if (
            !schifra::make_sequential_root_generator_polynomial(
                    field,
                    generator_polynomial_index,
                    generator_polynomial_root_count,
                    generator_polynomial)
            )
    {
        std::cout << "Error - Failed to create sequential root generator!" << std::endl;
        exit(1);
    }
}

std::vector<uint8_t> generate_random_data(const uint32_t mysize, std::mt19937_64 &rnd){
    std::vector<uint8_t> input;
    input.reserve(mysize);
    // Fill it with random data
    std::uniform_int_distribution<int16_t> distr(0,
                                                 std::numeric_limits<uint8_t>::max());
    // fill our input with random data
    std::cout << "===== Input data =====" << std::endl;
    for (size_t idx = 0; idx < mysize; ++idx) {
        uint8_t iiidata = static_cast<uint8_t> (distr(rnd));
        printf("%d ", iiidata);
        input.push_back(iiidata);
    }
    std::cout << std::endl << "===== Input data =====\n" << std::endl;

    return input;
}


/* Reed Solomon Code Parameters */
const std::size_t code_length = 255;    // The whole packet length.
const std::size_t fec_length  =  32;    // The number of redundant symbols.
const std::size_t data_length = code_length - fec_length;   // The number of data symbols.

/* Instantiate Encoder and Decoder (Codec) */
typedef schifra::reed_solomon::encoder<code_length,fec_length,data_length> encoder_t;
typedef schifra::reed_solomon::decoder<code_length,fec_length,data_length> decoder_t;

int main()
{
    check_field();

    const encoder_t encoder(field,generator_polynomial);
    const decoder_t decoder(field,generator_polynomial_index);

//    std::mt19937_64 rnd;
//    std::ifstream rand("/dev/urandom");
//    uint64_t seed = 0;
//    rand.read(reinterpret_cast<char *> (&seed), sizeof(seed));
//    rand.close();
//    rnd.seed(seed);

    std::string message = "An expert is someone who knows more and more about less and "
                          "less until they know absolutely everything about nothing";

    /* Pad message with nulls up until the code-word length */
    message.resize(code_length,0x00);

    /* Instantiate RS Block For Codec */
    schifra::reed_solomon::block<code_length,fec_length> block;

    /* Transform message into Reed-Solomon encoded codeword */
    if (!encoder.encode(message,block))
    {
        std::cout << "Error - Critical decoding failure! "
                  << "Msg: " << block.error_as_string()  << std::endl;
        return 1;
    }

    /* Add errors at every 3rd location starting at position zero */
    schifra::corrupt_message_all_errors00(block, 0, 3);

    if (!decoder.decode(block))
    {
        std::cout << "Error - Critical decoding failure!" << std::endl;
        return 1;
    }
    else if (!schifra::is_block_equivelent(block,message))
    {
        std::cout << "Error - Error correction failed!" << std::endl;
        return 1;
    }

    block.data_to_string(message);

    return 0;
}