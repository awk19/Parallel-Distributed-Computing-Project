#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <omp.h>
#include <time.h>
#define KEY 19

struct imgData {
	char* data;
	int len;
};

int fileCount(char* dir)
{
	int file_count = 0;
	DIR* dirp;
	struct dirent * entry;

	dirp = opendir(dir);
	while ((entry = readdir(dirp)) != NULL) 
	{
	    if (entry->d_type == DT_REG) 
	    { 
		 file_count++;
	    }
	}
	closedir(dirp);
	
	return file_count;
}

struct imgData* readFile(char* path)
{
	FILE* fp;
	
	struct imgData *img = malloc(sizeof(struct imgData));
	
	fp = fopen(path, "r");
	if(fp == NULL)
	{
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
 	img->len = ftell(fp);
 	fseek(fp, 0, SEEK_SET);
 	
 	img->data = malloc(img->len);
 	fread(img->data, img->len, 1, fp);
	
	fclose(fp);
	
	return img;
}

void writeFile(char* path, struct imgData *img)
{
	FILE* fp;
	
	fp = fopen(path, "w");
	if(fp == NULL)
	{
		return;
	}
	
	fwrite(img->data, 1, img->len, fp);
	
	fclose(fp);
}

void encryptImage(int i)
{
	char* path1 = malloc(50);
	char* path2 = malloc(50);
	sprintf(path1, "images/%d.jpg", i);
	sprintf(path2, "encryptedImages/%d.ecd", i);
	
	struct imgData* img = readFile(path1);
	if(img == NULL)
	{
		printf("%s not found\n", path1);
		free(path1);
		free(path2);
		return;
	}
	
	for(int i=0; i<img->len; i++)
	{
		img->data[i] += KEY; //Encrypting each character
	}
	
	writeFile(path2, img);
	
	printf("Image# %d encrypted successfully\n", i);
	
	free(path1);
	free(path2);
	free(img->data);
	free(img);
}

void decryptImage(int i)
{
	char* path1 = malloc(50);
	char* path2 = malloc(50);
	sprintf(path1, "encryptedImages/%d.ecd", i);
	sprintf(path2, "decryptedImages/%d.jpg", i);
	
	struct imgData* img = readFile(path1);
	if(img == NULL)
	{
		printf("%s not found\n", path1);
		free(path1);
		free(path2);
		return;
	}
	
	for(int i=0; i<img->len; i++)
	{
		img->data[i] -= KEY; //Decrypting each character
	}
	
	writeFile(path2, img);
	
	printf("Image# %d decrypted successfully\n", i);
	
	free(path1);
	free(path2);
	free(img->data);
	free(img);
}

void encryptAllImagesParallel()
{
	int n = fileCount("images");
	
	printf("\nStarting Parallel Encryption\n\n");
	
	omp_set_dynamic(0);
	#pragma omp parallel for num_threads(64)
	for(int i=1; i<=n; i++)
	{
		encryptImage(i);
	}
}

void decryptAllImagesParallel()
{
	int n = fileCount("encryptedImages");
	
	printf("\nStarting Parallel Decryption\n\n");
		
	omp_set_dynamic(0);
	#pragma omp parallel for num_threads(64)
	for(int i=1; i<=n; i++)
	{
		decryptImage(i);
	}
}

void encryptAllImagesSerial()
{
	int n = fileCount("images");
	
	printf("\nStarting Serial Encryption\n\n");
	
	for(int i=1; i<=n; i++)
	{
		encryptImage(i);
	}
}

void decryptAllImagesSerial()
{
	int n = fileCount("encryptedImages");
	
	printf("\nStarting Serial Decryption\n\n");
		
	for(int i=1; i<=n; i++)
	{
		decryptImage(i);
	}
}

int main()
{
	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);
	
	encryptAllImagesSerial();
	decryptAllImagesSerial();
	
	clock_gettime(CLOCK_REALTIME, &end);

	double timeDurationSerial = ((end.tv_sec - start.tv_sec) * 1000000) + ((end.tv_nsec - start.tv_nsec) / 1000);
	timeDurationSerial /= 1000000;
	
	clock_gettime(CLOCK_REALTIME, &start);
	
	encryptAllImagesParallel();
	decryptAllImagesParallel();
	
	clock_gettime(CLOCK_REALTIME, &end);

	double timeDurationParallel = ((end.tv_sec - start.tv_sec) * 1000000) + ((end.tv_nsec - start.tv_nsec) / 1000);
	timeDurationParallel /= 1000000;

	printf("Time Duration for Serial Encryption and Decryption is: %.2lf seconds\n", timeDurationSerial);
	printf("Time Duration for Parallel Encryption and Decryption is: %.2lf seconds\n", timeDurationParallel);
	
	return 0;
}
