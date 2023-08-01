#include <stdio.h>
#include <byteswap.h>

int main()
{
	int word1, word2;
	while(! feof(stdin)) {
		fread(&word1, 4, 1, stdin );
		word2 = bswap_32(word1);
		fwrite(&word2, 4, 1, stdout);
	}
}
