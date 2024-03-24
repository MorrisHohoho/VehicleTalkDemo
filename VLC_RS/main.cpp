/*
(**************************************************************************)
(*                                                                        *)
(*                                Schifra                                 *)
(*                Reed-Solomon Error Correcting Code Library              *)
(*                                                                        *)
(* Release Version 0.0.1                                                  *)
(* http://www.schifra.com                                                 *)
(* Copyright (c) 2000-2020 Arash Partow, All Rights Reserved.             *)
(*                                                                        *)
(* The Schifra Reed-Solomon error correcting code library and all its     *)
(* components are supplied under the terms of the General Schifra License *)
(* agreement. The contents of the Schifra Reed-Solomon error correcting   *)
(* code library and all its components may not be copied or disclosed     *)
(* except in accordance with the terms of that agreement.                 *)
(*                                                                        *)
(* URL: http://www.schifra.com/license.html                               *)
(*                                                                        *)
(**************************************************************************)
*/


/*
   Description: This example will demonstrate how to instantiate a Reed-Solomon
                encoder and decoder, add the full amount of possible erasures,
                correct the erasures, and output the various pieces of relevant
                information.
*/


#include <cstddef>
#include <iostream>
#include <string>

#include "schifra_galois_field.hpp"
#include "schifra_galois_field_polynomial.hpp"
#include "schifra_sequential_root_generator_polynomial_creator.hpp"
#include "schifra_reed_solomon_encoder.hpp"
#include "schifra_reed_solomon_decoder.hpp"
#include "schifra_reed_solomon_block.hpp"
#include "schifra_error_processes.hpp"
#include "schifra_utilities.hpp"

#define VLC_VERBOSE 1
#define VLC_FRMAE_LENGTH 32


int main(int argc, char *argv[]) {
    /* Timer */
    schifra::utils::timer timer;
    /* Finite Field Parameters */
    const std::size_t field_descriptor = 8;
    const std::size_t generator_polynomial_index = 120;
    const std::size_t generator_polynomial_root_count = 32;

    /* Reed Solomon Code Parameters */
    const std::size_t code_length = 160;
    const std::size_t fec_length = 32;
    const std::size_t data_length = code_length - fec_length;

    /* Instantiate Finite Field and Generator Polynomials */
    const schifra::galois::field field(field_descriptor,
                                       schifra::galois::primitive_polynomial_size06,
                                       schifra::galois::primitive_polynomial06);

    schifra::galois::field_polynomial generator_polynomial(field);

    if (
            !schifra::make_sequential_root_generator_polynomial(field,
                                                                generator_polynomial_index,
                                                                generator_polynomial_root_count,
                                                                generator_polynomial)
            ) {
        std::cout << "Error - Failed to create sequential root generator!" << std::endl;
        return 1;
    }

    /* Instantiate Encoder and Decoder (Codec) */
    typedef schifra::reed_solomon::shortened_encoder<code_length, fec_length, data_length> encoder_t;
    typedef schifra::reed_solomon::shortened_decoder<code_length, fec_length, data_length> decoder_t;

    const encoder_t encoder(field, generator_polynomial);
    const decoder_t decoder(field, generator_polynomial_index);

    std::string message = "SCUNO1koNavS4Ts/TsDrR A3<oSvoGt6^{KmWKS0Ld%;I7=a5m*WAPkr(L<!#+Q| y%Wc4Gp^Idj_ky5L[64j+Q9]ApsJrF#Jp-W";

    /* Pad message with nulls up until the code-word length */
    message.resize(code_length, 0x00);

#if VLC_VERBOSE
    std::cout<<"original:"<<std::endl;
    for (auto i:message)
    {
        printf("%d,",i);
    }
    printf("\n");
#endif

    /* Instantiate RS Block For Codec */
    schifra::reed_solomon::block<code_length, fec_length> block;

    timer.start();
    /* Transform message into Reed-Solomon encoded codeword */
    if (!encoder.encode(message, block)) {
        std::cout << "Error - Critical encoding failure! "
                  << "Msg: " << block.error_as_string() << std::endl;
        return 1;
    }
    timer.stop();
    std::cout<<"RS encode time:"<<timer.time()<<std::endl;

#if VLC_VERBOSE
    std::cout<<"encoded:"<<std::endl;
    for (auto i:block.data)
    {
        printf("%d,",i);
    }
    printf("\n");
#endif


    timer.start();
    if (!decoder.decode(block)) {
        std::cout << "Error - Critical decoding failure! "
                  << "Msg: " << block.error_as_string() << std::endl;
        return 1;
    }
    timer.stop();
    std::cout<<"RS decode time:"<<timer.time()<<std::endl;

    block.data_to_string(message);

#if!VLC_VERBOSE
    std::cout << "Corrected Message: [" << message << "]" << std::endl;

    std::cout << "Encoder Parameters [" << encoder_t::trait::code_length << ","
              << encoder_t::trait::data_length << ","
              << encoder_t::trait::fec_length << "]" << std::endl;

    std::cout << "Decoder Parameters [" << decoder_t::trait::code_length << ","
              << decoder_t::trait::data_length << ","
              << decoder_t::trait::fec_length << "]" << std::endl;
#endif
    return 0;
}