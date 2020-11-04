%language "c++"
%require "3.7.1"
%defines

%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
	// requires
}

%param { driver& drv } // The parsing context.

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
#include "driver.hh"
}

%token EOF    0    "end of file"

%start midi_file

%%

/* TODO:
	header_chunk
	track_chunk
 */


midi_file: header_chunk track_chunks EOF
;

track_chunks: track_chunk
	| track_chunks track_chunk
;
