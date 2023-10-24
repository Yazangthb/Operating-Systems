	#include <stdlib.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <ctype.h>
	#include <sys/mman.h>
	#include <stdio.h>
	#define FILE_SIZE (500 * 1024 * 1024) // 500 MiB
	 
	int main()
	{
	    // Create an empty file named "text.txt"
	    int fileDescriptor = open("text.txt", O_CREAT | O_WRONLY, 0666);
	    if (fileDescriptor == -1)
	    {
	        perror("open");
	        exit(1);
	    }
	 
	    // Open /dev/random for generating random characters
	    int randomDescriptor = open("/dev/random", O_RDONLY);
	    if (randomDescriptor == -1)
	    {
	        perror("open");
	        close(fileDescriptor);
	        exit(1);
	    }
	 
	    // Read and process characters from /dev/random
	    char buffer[1024];
	    char *text = (char *)malloc(FILE_SIZE);
	    size_t textLength = 0;
	 
	    while (textLength < FILE_SIZE)
	    {
	        ssize_t bytesRead = read(randomDescriptor, buffer, sizeof(buffer));
	        if (bytesRead == -1)
	        {
	            perror("read");
	            close(randomDescriptor);
	            close(fileDescriptor);
	            free(text);
	            exit(1);
	        }
	 
	        for (ssize_t i = 0; i < bytesRead && textLength < FILE_SIZE; i++)
	        {
	            if (isprint(buffer[i]))
	            {
	                text[textLength] = buffer[i];
	                textLength++;
	                if (textLength % 1024 == 0)
	                {
	                    text[textLength] = '\n';
	                    textLength++;
	                }
	            }
	        }
	    }
	 
	    // Write the filtered text to "text.txt"
	    ssize_t bytesWritten = write(fileDescriptor, text, textLength);
	    if (bytesWritten == -1)
	    {
	        perror("write");
	        close(randomDescriptor);
	        close(fileDescriptor);
	        free(text);
	        exit(1);
	    }
	 
	    // Close file descriptors
	    close(randomDescriptor);
	    close(fileDescriptor);
	 
	    // Open "text.txt" for memory mapping
	    fileDescriptor = open("text.txt", O_RDWR);
	    if (fileDescriptor == -1)
	    {
	        perror("open");
	        free(text);
	        exit(1);
	    }
	 
	    // Determine the page size
	    long pageSize = sysconf(_SC_PAGESIZE);
	 
	    // Map the file in chunks
	    off_t offset = 0;
	    char *mapped;
	    size_t totalCapitalLetters = 0;
	 
	    while (offset < textLength)
	    {
	        size_t chunkSize = pageSize * 1024;
	        if (textLength - offset < chunkSize)
	        {
	            chunkSize = textLength - offset;
	        }
	 
	        mapped = mmap(NULL, chunkSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, offset);
	        if (mapped == MAP_FAILED)
	        {
	            perror("mmap");
	            close(fileDescriptor);
	            free(text);
	            exit(1);
	        }
	 
	        // Count and replace capital letters
	        for (size_t i = 0; i < chunkSize; i++)
	        {
	            if (isupper(mapped[i]))
	            {
	                mapped[i] = tolower(mapped[i]);
	                totalCapitalLetters++;
	            }
	        }
	 
	        munmap(mapped, chunkSize);
	        offset += chunkSize;
	    }
	 
	    close(fileDescriptor);
	 
	    // Print the total number of capital letters
	    printf("Total capital letters: %zu\n", totalCapitalLetters);
	 
	    // Clean up and exit
	    free(text);
	    return 0;
	}
