#include <stdio.h>
#include "common.h"
#include "partial.h"
#include "pzip.h"

void callback(ZipInfo* info, CDFile* file, size_t progress) {
	int percentDone = progress * 100/file->compressedSize;
	//fprintf(stderr, "Getting: %d%%\n", percentDone);
}

int partialZip(char *url, char *desiredFile, char *outFile) {
    
	/*	
     argv[0] = url
     argv[1] = desiredFile
     argv[2] = outDir
	*/

	char* extract = desiredFile;
    
    //char fname[strlen(desiredFile) + sizeof("file://")];
	
    char fname[strlen(url + sizeof("file://"))];
    
    char* outfile = outFile;

	if(strstr(url, "://") == NULL) {
		strcpy(fname, "file://");
	}

	strcat(fname, url);
    
   // printf("fname %s url %s\n", fname, url);

	ZipInfo* info = PartialZipInit(url);
	if(!info) {
		fprintf(stderr, "Cannot open %s\n", fname);
		return 0;
	}

	PartialZipSetProgressCallback(info, callback);

	CDFile* file = PartialZipFindFile(info, extract);
	if(!file) {
		fprintf(stderr, "Cannot find %s in %s\n", extract, fname);
		return 0;
	}

	unsigned char* data = PartialZipGetFile(info, file);
	int dataLen = file->size;

	//PartialZipRelease(info);

	data = realloc(data, dataLen + 1);
	data[dataLen] = '\0';

	FILE* out;
	out = fopen(outfile, "w");

	if (out == NULL) {
		fprintf(stderr, "Failed to open file");
		exit(-1);
	}

	int done = 0;
	done = fwrite(data, sizeof(char), dataLen, out);

	fclose(out);

	free(data);

	return 0;
}

