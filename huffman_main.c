#include <stdio.h>
#include <stdlib.h>
#include "huffman.h" 

static uint8_t *_readFile(const char *path)
{
	FILE *fp = fopen(path, "rb");
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	uint8_t *content = calloc(size + 1, sizeof(*content));
	fread(content, size, 1, fp);
	fclose(fp);
	content[size] = '\0';

	return content;
}	

static void _fileCompression(const char *srcPath, const char *destPath, uint64_t freq[256], uint8_t *content)
{
	Node *pq = makeHuffmanPq(freq);
	HuffTreeNode *root = makeHuffmanTree(pq);
	BitWriter writer = openBitWriter(destPath);
	writeCompressedBytes(root, &writer, content);

	closeBitWriter(&writer);
	free(content);
	destroyHuffmanTree(&root);
}

int main(int argc, char *argv[])
{
	uint64_t freqs[256] = {0};
	const char *error = NULL;
	const char *srcPath = argv[1];
	const char *destPath = argv[2];
	uint8_t *content = _readFile(srcPath);

	if (!calcFrequencies(freqs, srcPath, &error)) {
		printf("%s\n", error);	
		return EXIT_FAILURE;
	}

	_fileCompression(srcPath, destPath, freqs, content);

	return EXIT_SUCCESS;
}
