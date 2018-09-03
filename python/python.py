from ctypes import*
mydll = cdll.LoadLibrary("C:\\novadll\\libindt_dll.dll")

class TABELA(Structure):
    _fields_ = [
        ("id", c_char),
        ("size", c_long),
        ("qtde_programs", c_char),
        ("net_name", c_char),
        ("program_number", c_char),
        ("program_type", c_char),
        ("service_number", c_char),
        ("net_number", c_char),
        ("header_body", c_char_p),
        ("table_data", c_char_p),
        ("header_size", c_long)
    ]

tabela = TABELA()
result = mydll.initBynaryTable(b"C:\\novadll\\teste.bin")
if result == 0:
    proxima = 1
    tamanho = mydll.getTamanhoArquivo()
    data = 1

    while data != 0:
        data = mydll.getTable_data(proxima)
        tam = mydll.getTable_size()
        mydll.showHex(data,tam)
        print("\n")
        proxima += 1
    tabela.header_body = mydll.getHeader_body()
    tabela.header_size = mydll.getHeader_size()
    print("Header:")
    mydll.showHex(tabela.header_body,tabela.header_size)
    print("\nHeader size: ", tabela.header_size)
    
    tabela.id = 0x0A #APT

    proxima = 1
    data = 1
    while data != 0:
        data = mydll.findTable(0x0A,proxima)
        if data != 0:
            tabela.table_data = data
            tabela.size = mydll.getTable_size()
            tabela.qtde_programs = mydll.getQuantity_programs(proxima)
            tabela.net_name = mydll.getNet_name(proxima)
            mydll.showTabela(tabela)
        proxima += 1
    print("\nTabelas APT encontradas: ", proxima - 2)
    
    tabela.id = 0x0B #MPT
    proxima = 1
    data = 1
    while data != 0:
        data = mydll.findTable(0x0B,proxima)
        if data != 0:
            tabela.table_data = data
            tabela.size = mydll.getTable_size()
            tabela.program_number = mydll.getProgram_number(proxima)
            tabela.program_type = mydll.getProgram_type(proxima)
            mydll.showTabela(tabela)
        proxima += 1
    print("\nTabelas MPT encontradas: ", proxima - 2)

    tabela.id = 0x0E #TBA
    proxima = 1
    data = 1
    while data != 0:
        data = mydll.findTable(0x0E,proxima)
        if data != 0:
            tabela.table_data = data
            tabela.size = mydll.getTable_size()
            tabela.service_number = mydll.getService_number(proxima)
            tabela.net_number = mydll.getNet_number(proxima)
            mydll.showTabela(tabela)
        proxima += 1
    print("\nTabelas TBA encontradas: ", proxima - 2)
    
    tabela.id = 0x0C #ITN
    proxima = 1
    data = 1
    while data != 0:
        data = mydll.findTable(0x0C,proxima)
        if data != 0:
            tabela.table_data = data
            tabela.size = mydll.getTable_size()
            mydll.showTabela(tabela)
        proxima += 1
    print("\nTabelas ITN encontradas: ", proxima - 2)
    
    tabela.id = 0x0D #ATB
    proxima = 1
    data = 1
    while data != 0:
        data = mydll.findTable(0x0D,proxima)
        if data != 0:
            tabela.table_data = data
            tabela.size = mydll.getTable_size()
            mydll.showTabela(tabela)
        proxima += 1
    print("\nTabelas ATB encontradas: ", proxima - 2)
    
    tabela.id = 0x0F #TAP
    proxima = 1
    data = 1
    while data != 0:
        data = mydll.findTable(0x0F,proxima)
        if data != 0:
            tabela.table_data = data
            tabela.size = mydll.getTable_size()
            mydll.showTabela(tabela)
        proxima += 1
    print("\nTabelas TAP encontradas: ", proxima - 2)

mydll.closeBynaryTable()
