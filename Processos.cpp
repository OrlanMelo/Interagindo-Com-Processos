#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <string>
#include <iostream>
#include <tchar.h>
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"psapi.lib")

using namespace std;

class cFuncoes
{
private:

	HANDLE Snapshot;

	DWORD Identificadores[MAX_PATH], lpcbNeeded, Processos;

	HANDLE Identificador;
	HANDLE Identificador2;

public:

	void EnumerarProcessos()
	{
		int Quantidade = 0;

		if (EnumProcesses(Identificadores, sizeof(Identificadores), &lpcbNeeded) == NULL)
		{
			cout << "Ocorreu um erro ao obter o identificador de processo.\n";
		}
		else
		{
			Processos = lpcbNeeded / sizeof(DWORD);
			for (int i = 0; i < Processos; i++)
			{
				Quantidade = i;
				Funcoes.ObterNomesDeProcessos(Identificadores[i]);
				Funcoes.ObterLocalDeProcessos(Identificadores[i]);
			}
		}

		cout << "\n";
		cout << "Quantidade: " << Quantidade << " processos.\n";

		CloseHandle(Identificador);
	}

	void ObterNomesDeProcessos(DWORD Id)
	{
		TCHAR Nome[MAX_PATH] = TEXT("Desconhecido");

		if (Identificador = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, Id))
		{
			GetModuleBaseName(Identificador, 0, Nome, sizeof(Nome));
			_tprintf(L"Processo: %s\n", Nome);
		}
		else
		{
			//cout << "Ocorreu um erro ao obter o identificador de processo..\n" << GetLastError();
		}
	}

	void ObterLocalDeProcessos(DWORD Id)
	{
		TCHAR Local[MAX_PATH] = TEXT("Local desconhecido");

		if (Identificador2 = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, Id))
		{
			GetModuleFileNameEx(Identificador, 0, Local, sizeof(Local));
			_tprintf(L"Origem: %s\n", Local);
		}
	}

	void FinalizarProcesso(wstring Executavel)
	{
		Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
		if (Snapshot == INVALID_HANDLE_VALUE)
		{
			cout << "Identificador inválido para a operação solicitada.." << GetLastError();
		}
		else
		{
			PROCESSENTRY32 Entrada;
			Entrada.dwSize = sizeof(PROCESSENTRY32);

			Process32First(Snapshot, &Entrada);

			do
			{
				wstring processo = Entrada.szExeFile;

				if (processo == Executavel == true)
				{
					HANDLE IdentificadorEXE;
					if (IdentificadorEXE = OpenProcess(PROCESS_ALL_ACCESS, 0, Entrada.th32ProcessID))
					{
						TerminateProcess(IdentificadorEXE, PROCESS_TERMINATE);
						CloseHandle(Snapshot);
						CloseHandle(IdentificadorEXE);
					}
					else
					{
						cout << "Não foi possível obter o identificador de processo.." << GetLastError();
					}
				}
			} while (Process32Next(Snapshot, &Entrada));
		}
	}

}Funcoes;

int main()
{

	cout << "O assistente está verificando processos...\n";

	//Dentro desta função contém outras APIs.
	//APIs incluídas: Obter o nome de cada processo e local do processo no sistema de arquivos.
	Funcoes.EnumerarProcessos();

	//Os nomes de processos diferenciam maiúsculas e minúsculas.
	Funcoes.FinalizarProcesso(L"EXCEL.EXE");

	system("pause");
}
