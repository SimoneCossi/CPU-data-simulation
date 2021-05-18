GeneraFile: GeneratoreFileASD.c Makefile
    gcc -ansi -Wall -O GeneratoreFileASD.c -o GeneratoreFileASD.exe

CompilaProgetto: ProgettoASD.c Makefile
    gcc -ansi -Wall -O ProgettoASD.c -o ProgettoASD.exe