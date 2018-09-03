#ifndef INDT_DLL_H
#define INDT_DLL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILDING_INDT_DLL
#define INDT_DLL __declspec(dllexport)
#else
#define INDT_DLL __declspec(dllimport)
#endif


//--------- Global Variable Declaration ----------
struct Tabela {
	char  id;
	long  size;
	char  qtde_programs;
	char  net_name;
	char  program_number;
	char  program_type;
	char  service_number;
	char  net_number;
	char *header_body;
	char *table_data;
	long  header_size;
};
//--------- Global Variable Declaration ----------

//------- Library Functions for binary file  --------

// Initialize and load in 'buffer' the file content specified by 'path'.
// Set 'lSize' to valid size file content.
int INDT_DLL initBynaryTable(char *path);

// Close a file specified by 'path' in function initBynaryTable()
void INDT_DLL closeBynaryTable(void);

// Achieved size file specified by 'path' in function initBynaryTable()
long INDT_DLL getTamanhoArquivo(void);

// Achieved table body between tag's 0x48 and 0xAA (tag's not include)
// and set size table ('table_size')
char * INDT_DLL getTable_data(int next);

// Achieved size table from function getTable_data()
// The getTable_data() function must be called first
long  INDT_DLL getTable_size(void);

// Achieved header body between tag's  0x47 and 0xDD (tag's not include)
// and set size header ('header_size')
char * INDT_DLL getHeader_body(void);

// Achieved size header from function getHeader_body()
// The getHeader_body() function must be called first
long INDT_DLL getHeader_size(void);

// Looking for in a 'buffer' the 'next' (ordinal) occurrence
// the specified table and return your content
// Return 'NULL' if not found table.
char * INDT_DLL findTable(char tableId, int next);

// Show 'buffer_bin' contents in hex the 'size' first bytes
void INDT_DLL showHex(char *buffer_bin, long size);


void INDT_DLL showTabela(struct Tabela tab);

//For APT TABLE:
char  INDT_DLL getQuantity_programs(int next);
char  INDT_DLL getNet_name(int next);

//For MPT TABLE:
char  INDT_DLL getProgram_number(int next);
char  INDT_DLL getProgram_type(int next);

//For TBA TABLE
char  INDT_DLL getService_number(int next);
char  INDT_DLL getNet_number(int next);
//------- Library Functions for binary file  --------


//--------- Functions for internal use ---------
char * getBody(char *binBuffer, char head, char tail, int next);
long getSizeFile(void);
//--------- Functions for internal use ---------

#ifdef __cplusplus
}
#endif

#endif  // INDT_DLL_H



//============================================================================
// Name        : indt.cpp
// Author      : Luiz Claudio Menezes Atayde
// Version     : 0.0.1
// Copyright   : Your copyright notice
// Description : read a binary file and shown the data
//============================================================================

/*

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

using namespace std;

#ifndef INDT_DLL_H
#define INDT_DLL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILDING_INDT_DLL
#define INDT_DLL __declspec(dllexport)
#else
#define INDT_DLL __declspec(dllimport)
#endif

#define APT 0x0A
#define MPT 0x0B
#define ITN 0x0C
#define ATB 0x0D
#define TBA 0x0E
#define TAP 0x0F

struct Tabela {
	char  id;
	long  size;
	char  qtde_programs;
	char  net_name;
	char  program_number;
	char  program_type;
	char  service_number;
	char  net_number;
	char *header_body;
	char *table_data;
	long  header_size;
};

#ifdef __cplusplus
}
#endif

class INDT_DLL BinaryTable{
public:
	//constructor
	BinaryTable(void);

	//inicializa e carrega em 'buffer' o conteudo do arquivo especificado em 'path'.
	//seta 'lSize' para o tamanho do conteudo valido do arquivo
	int initBynaryTable(char *path);

	// Procura em 'buffer' a n-enesima ('next') ocorrencia
	// da tabela especificada e retorna o seu conteudo.
	// Retorna NULL quando tabela nao encontrada
	char * findTable(char tableId, int next);

	void closeBynaryTable(void);

	long getTamanhoArquivo(void);

	long  getTable_size(void);

	long getHeader_size(void);

	char  getQuantity_programs(int next);

	char  getNet_name(int next);

	char  getProgram_number(int next);

	char  getProgram_type(int next);

	char  getService_number(int next);

	char  getNet_number(int next);

	//obtem o corpo do cabecalho (tag's 0x47 e 0xDD nao inclusas)
	//e seta o tamanho do cabecalho (header_size)
	char * getHeader_body(void);

	//obtem o corpo da tabela (tag's 0x48 e 0xAA nao inclusas)
	//e seta o tamanho da tabela (table_size)
	char * getTable_data(int next);

	void showHex(char *buffer_bin, long size);

	void showTabela(struct Tabela tab);

//table identification
// code		Description
// 0xA 		APT
// 0xB		MPT
// 0xC		ITN
// 0xD		ATB
// 0xE		TBA
// 0xF		TAP

private:

	char  table_id;				// codigo de identificacao (apos 0x48)
	long  table_size;			// tamanho do corpo em bytes (entre 0x48 e 0xAA)
	char  quantity_programs;	// numero de programas (primeiro byte do corpo)
	char  net_name;				// nome da rede (segundo byte do corpo)
	char  program_number;		// numero do programa (primeiro byte do corpo)
	char  program_type;			// tipo de programa (segundo byte do corpo)
	char  service_number;		// numero do servico (primeiro byte do corpo)
	char  net_number;			// numero da rede (segundo byte do corpo)
	char *header_body;			// corpo do cabeçalho
	char *table_data;			// dados da tabela
	long  header_size;			// tamanho do cabecalho em bytes (entre0x47 e 0xDD)

	FILE *pFile;				// handle para o arquivo
	char *buffer;				// para armazenar conteudo do aquivo
	char *body;					// para armazenar o conteudo entre tags
	long lSize;					// tamanho do arquivo
	bool initialized;			// flag para status da inicializacao

	long aux_size;				//tamanho de corpo (table body or header body)

	//obtem o tamanho do arquivo e guarda em lSize (end of file: 0xFF 0xFF).
	long getSizeFile(void);

	//procura em binBuffer a n-enesima ('next') ocorrencia do conteudo entre as tags 'head' e 'tail'.
	//seta a variavel aux_size com o tamanho do conteudo.
	//No sucesso retorna o conteudo entre as tag's. Retorna NULL no erro.
	char * getBody(char *binBuffer, char head, char tail, int next);

};

#endif  // INDT_DLL_H
*/
