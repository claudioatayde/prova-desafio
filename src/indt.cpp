//============================================================================
// Name        : indt.cpp
// Author      : Luiz Claudio Menezes Atayde
// Version     : 0.0.1
// Copyright   : Your copyright notice
// Description : read a binary file and shown the data
//============================================================================

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

using namespace std;

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

class BinaryTable{
public:
	//constructor
	BinaryTable(void){
		table_id = 0;			// codigo de identificacao (apos 0x48)
		table_size = 0;			// tamanho do corpo em bytes (entre 0x48 e 0xAA)
		quantity_programs = 0;	// numero de programas (primeiro byte do corpo)
		net_name = 0;			// nome da rede (segundo byte do corpo)
		program_number = 0;		// numero do programa (primeiro byte do corpo)
		program_type = 0;		// tipo de programa (segundo byte do corpo)
		service_number = 0;		// numero do servico (primeiro byte do corpo)
		net_number = 0;			// numero da rede (segundo byte do corpo)
		header_body = 0x00;		// corpo do cabeçalho
		table_data = 0x00;		// dados da tabela

		buffer = 0x00;			// para armazenar conteudo do aquivo
		body = 0x00;			// para armazenar o conteudo entre tags
		lSize = 0;				// tamanho do arquivo / conteudo valido.
		initialized = false;	// nao inicializado
	}

	//inicializa e carrega em 'buffer' o conteudo do arquivo especificado em 'path'.
	//seta 'lSize' para o tamanho do conteudo valido do arquivo
	int initBynaryTable(char *path){
		int codret = 0;

		size_t result;
		pFile = fopen(path, "rb" ); //open in binary mode
		if (pFile == NULL) {
			cout << "Erro na abertura do arquivo";
			codret = -1; // erro na abertura do arquivo
		}
		else{
			// armazena em lSize o numero de bytes do arquivo apontado por pFile
			fseek(pFile, 0, SEEK_END); 	//aponta para o final do arquivo
			lSize = ftell (pFile);		//guarda em lSize a posicao final do arquivo
			fseek(pFile, 0, SEEK_SET);	//aponta para o inicio do arquivo

			buffer = (char*) malloc (sizeof(char)*lSize);
			if(buffer == NULL) {
				fputs ("Erro alocando memoria",stderr);
				codret = -2; //error alocando memoria
			}
			//copia o conteudo do arquivo para buffer
			result = fread(buffer,1,lSize,pFile);
			if((long)result != lSize){
				fputs("Erro copiando para o buffer",stderr);
				codret = -3; //erro copiando para o buffer
			}
			if(codret == 0){
				//atualiza o valor de lSize para o tamanho do conteudo valido
				getSizeFile();
				initialized = true;
//o codigo seguinte serve para visulizar o conteudo em hex
//				while(buffer != NULL && cont < lSize){
//					sprintf(strHex,"%.2X",0x000000FF & (buffer[cont]));
//					cout << ' ' << strHex;
//					cont++;
//				}
			}
		}
		return codret;
	}
	// Procura em 'buffer' a n-enesima ('next') ocorrencia
	// da tabela especificada e retorna o seu conteudo.
	// Retorna NULL quando tabela nao encontrada
	char * findTable(char tableId, int next){
		char *content = NULL;
		int contTab = 1, contId = 1;

		do {
			content = getBody(buffer, 0x48, 0xAA, contTab);
			if(content != NULL){
				if(content[0] == tableId){
					if(contId == next){
						table_size = aux_size;
						return content;
					}
					contId++;
				}
				contTab++;
			}
		} while(contId <= next && content != NULL);
		return content;
	}

	void closeBynaryTable(void){
		fclose(pFile);
	}

	long getTamanhoArquivo(void){
		return getSizeFile();
	}

	long  getTable_size(void){
		return table_size;
	}

	long getHeader_size(void){
		return header_size;
	}
	char  getQuantity_programs(int next){
		char *tabela;
		quantity_programs = 0;
		tabela = findTable(APT,next);
		if(tabela != NULL){
			quantity_programs = 0x000000FF & *(tabela + 1);
		}
		return quantity_programs;
	}

	char  getNet_name(int next){
		char *tabela;
		net_name = 0;
		tabela = findTable(APT,next);
		if(tabela != NULL){
			net_name = 0x000000FF & *(tabela + 2);
		}
		return net_name;
	}

	char  getProgram_number(int next){
		char *tabela;
		program_number = 0;
		tabela = findTable(MPT,next);
		if(tabela != NULL){
			program_number = 0x000000FF & *(tabela + 1);
		}
		return program_number;
	}

	char  getProgram_type(int next){
		char *tabela;
		program_type = 0;
		tabela = findTable(MPT,next);
		if(tabela != NULL){
			program_type = 0x000000FF & *(tabela + 2);
		}
		return program_type;
	}

	char  getService_number(int next){
		char *tabela;
		service_number = 0;
		tabela = findTable(TBA,next);
		if(tabela != NULL){
			service_number = 0x000000FF & *(tabela + 1);
		}
		return service_number;
	}

	char  getNet_number(int next){
		char *tabela;
		net_number = 0;
		tabela = findTable(TBA,next);
		if(tabela != NULL){
			net_number = 0x000000FF & *(tabela + 2);
		}
		return net_number;
	}

	//obtem o corpo do cabecalho (tag's 0x47 e 0xDD nao inclusas)
	//e seta o tamanho do cabecalho (header_size)
	char * getHeader_body(void){
		header_body = getBody(buffer, 0x47, 0xDD, 1);
		header_size = aux_size;

		return header_body;
	}
	//obtem o corpo da tabela (tag's 0x48 e 0xAA nao inclusas)
	//e seta o tamanho da tabela (table_size)
	char * getTable_data(int next){
		table_data = getBody(buffer, 0x48, 0xAA, next);
		table_size = aux_size;

		return table_data;
	}

	void showHex(char *buffer_bin, long size){
		char strHex[] = {"  "};
		long cont = 0;

		while(buffer_bin != NULL && cont < size){
			sprintf(strHex,"%.2X",0x000000FF & (buffer_bin[cont]));
			cout << ' ' << strHex;
			cont++;
		}
	}

	void showTabela(struct Tabela tab){
//		cout << "\nCabecalho: ";
//		showHex(tab.header_body, tab.header_size);
//		std::cout <<"\nTamanho do cabecalho: "<< std::dec << (0x000000FF & tab.header_size);
		std::cout <<"\nIdentificacao da tabela: "<< std::hex << (0x000000FF & tab.id);
		cout << "\nCorpo da tabela: ";
		showHex(tab.table_data, tab.size);
		std::cout <<"\nTamanho da tabela: "<< std::dec << (0x000000FF & tab.size);

		if(tab.id == APT){
			std::cout <<"\nQuantidade de programas: "<< std::dec << (0x000000FF & tab.qtde_programs);
			std::cout <<"\nNome da rede: "<< std::hex << (0x000000FF & tab.net_name);
		}
		if(tab.id == MPT){
			std::cout <<"\nNumero do programa: "<< std::dec << (0x000000FF & tab.program_number);
			std::cout <<"\nTipo programa: "<< std::hex << (0x000000FF & tab.program_type);
		}
		if(tab.id == TBA){
			std::cout <<"\nNumero do servico: "<< std::hex << (0x000000FF & tab.service_number);
			std::cout <<"\nNumero da rede: "<< std::hex << (0x000000FF & tab.net_number);
		}
	}
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
	long getSizeFile(void){
		char *begin, *end;
		if(buffer != NULL){
			do{
				//procura caractere 0xFF
				begin = (char*)memchr(buffer, 0xFF, lSize);
				if(begin != NULL){
					//procura caractere 0xFF na proxima posicao
					end = (char*)memchr(begin+1, 0xFF, 1);
					if(end != NULL){
						lSize = (long)(end - buffer + 1);
						break;
					}
				}
			} while(begin != NULL);
		}
		return lSize;
	}

	//procura em binBuffer a n-enesima ('next') ocorrencia do conteudo entre as tags 'head' e 'tail'.
	//seta a variavel aux_size com o tamanho do conteudo.
	//No sucesso retorna o conteudo entre as tag's. Retorna NULL no erro.
	char * getBody(char *binBuffer, char head, char tail, int next){
		char *begin = NULL, *end = NULL;
		int cont = 0;

		end = binBuffer;
		do{
			body = NULL;
			aux_size = 0;
			table_size = 0;
			if(initialized == true){
				begin = (char*)memchr(end, head, lSize);
				if(begin != NULL){
					end = (char*)memchr(begin+1, tail, lSize-(begin-binBuffer)-1);
					if(end != NULL){
						table_size = (char)(end - begin);
						body = (char*)malloc(end - begin);
						memset(body,0x00,end - begin);
						memcpy(body, begin + 1, (size_t)(end - begin) - 1);
						aux_size = (long)(end - begin - 1);
						cont++;
					}
				}
			}
		}while(cont < next && body != NULL);
		return body;
	}

};
//#define APT 0xA
//#define MPT 0xB
//#define ITN 0xC
//#define ATB 0xD
//#define TBA 0xE
//#define TAP 0xF


int main(){
	BinaryTable table;
	char *pchar;
	char file[] = {"teste.bin"};
	Tabela tabela1;
	int tamanho, proxima = 1;

	if(table.initBynaryTable(file) == 0){

		tamanho = table.getTamanhoArquivo();
		pchar = (char*) malloc(sizeof(char)*tamanho);
		memset(pchar,0x00,tamanho);

		//mostra o conteudo das tabelas, incluindo seus id's
		cout << "Conteudo das tabelas do arquivo\n";
		do{
			pchar = table.getTable_data(proxima);
			table.showHex(pchar,(int)table.getTable_size());
			cout << "\n";
			proxima++;
		}while(pchar != NULL);

		tabela1.header_body = table.getHeader_body();
		tabela1.header_size = table.getHeader_size();

		cout << "Cabecalho: ";
		table.showHex(tabela1.header_body,tabela1.header_size);
		std::cout <<"\nTamanho do cabecalho: "<< std::dec << (0x000000FF & tabela1.header_size);

		proxima = 1;
		tabela1.id = APT;
		do{
			pchar = table.findTable(tabela1.id,proxima);
			if(pchar != NULL){
				tabela1.table_data = pchar;
				tabela1.size = table.getTable_size();
				tabela1.qtde_programs = table.getQuantity_programs(proxima);
				tabela1.net_name = table.getNet_name(proxima);
				table.showTabela(tabela1);
			}
			proxima++;
		}while(pchar != NULL);

		cout << "\nTabelas APT encontradas: "<< proxima - 2<< "\n-----------------";
		proxima = 1;
		tabela1.id = MPT;
		do{
			pchar = table.findTable(tabela1.id,proxima);
			if(pchar != NULL){
				tabela1.table_data = pchar;
				tabela1.size = table.getTable_size();
				tabela1.program_number = table.getProgram_number(proxima);
				tabela1.program_type = table.getProgram_type(proxima);
				table.showTabela(tabela1);
			}
			proxima++;
		}while(pchar != NULL);

		cout << "\nTabelas MPT encontradas: "<< proxima - 2<< "\n-----------------";
		proxima = 1;
		tabela1.id = TBA;
		do{
			pchar = table.findTable(tabela1.id,proxima);
			if(pchar != NULL){
				tabela1.table_data = pchar;
				tabela1.size = table.getTable_size();
				tabela1.service_number = table.getService_number(proxima);
				tabela1.net_number = table.getNet_number(proxima);
				table.showTabela(tabela1);
			}
			proxima++;
		}while(pchar != NULL);

		cout << "\nTabelas TBA encontradas: "<< proxima - 2<< "\n-----------------";
		proxima = 1;
		tabela1.id = ITN;
		do{
			pchar = table.findTable(tabela1.id,proxima);
			if(pchar != NULL){
				tabela1.table_data = table.getTable_data(proxima);
				tabela1.size = table.getTable_size();
				table.showTabela(tabela1);
			}
			proxima++;
		}while(pchar != NULL);

		cout << "\nTabelas ITN encontradas: "<< proxima - 2<< "\n-----------------";
		proxima = 1;
		tabela1.id = ATB;
		do{
			pchar = table.findTable(tabela1.id,proxima);
			if(pchar != NULL){
				tabela1.table_data = table.getTable_data(proxima);
				tabela1.size = table.getTable_size();
				table.showTabela(tabela1);
			}
			proxima++;
		}while(pchar != NULL);

		cout << "\nTabelas ATB encontradas: "<< proxima - 2<< "\n-----------------";
		proxima = 1;
		tabela1.id = TAP;
		do{
			pchar = table.findTable(tabela1.id,proxima);
			if(pchar != NULL){
				tabela1.table_data = table.getTable_data(proxima);
				tabela1.size = table.getTable_size();
				table.showTabela(tabela1);
			}
			proxima++;
		}while(pchar != NULL);
	}
	table.closeBynaryTable();

	return 0;
}
