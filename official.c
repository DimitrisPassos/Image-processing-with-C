#include "bmp.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <ctype.h>



struct rgb** ReadBMP(char* filename,struct bmp_info *theImage,struct bmp_header *header);

void printRgb(struct rgb **a);

struct rgb** memoryLock(struct rgb** theImage,int width,int height);

void writeFile(char *name,struct rgb** some,struct bmp_info *info);

void printImageInfo(char *name,struct bmp_info *theImage,struct bmp_header *header);

void writeMirrorFile(char *name,struct rgb** some,struct bmp_info *info);

void writeReducedFile(char *name,struct rgb** some,struct bmp_info *info,int numb);

void writePercentageFile(char *name,struct rgb** some,struct bmp_info *info,int numb);

int FindNearestColor(struct rgb **more, struct bmp_info *header);

void mergeSort(int arr[], int l, int r);

void merge(int arr[], int l, int m, int r);

void mergeSort(int arr[], int l, int r);

void printArray(int A[], int size);

int FindNearestColor2(char *red ,char *blue , char *green, struct bmp_info *header);

void writeMirrorHorizontalyFile(char *name,struct rgb** some,struct bmp_info *info);







int  main (int argc, char *argv[])
{
    
    /* 
 Passos Dimitrios
 AM: 2024201000083
 tst10083@uop.gr

 Lagakos Dimitrios Rafail
 AM: 2022201400116
 dit14116@uop.gr
*/
    
    
    
    struct bmp_info *theImage = (struct bmp_info *)malloc(sizeof(struct bmp_info));// Periexei struct bmp_info_header bmiHeader;

    struct bmp_header *header=(struct bmp_header *)malloc(sizeof(struct bmp_header)); // to Header
    struct rgb** colorMap;

    if (argc == 3)
    {
        char *first = argv[1];
        
        if (strstr(first,"-a"))
        {
            
            char *filename = argv[2];
            char *add = ".bmp";
            strcat(filename,add);
        
            printImageInfo(filename,theImage,header);
            
        }
        
        
    }else if(argc == 4)
        {
            char *first = argv[1];
            if (strstr(first,"-fv"))
            {
                
                char *filenameInput = strdup(argv[2]);
                char *add = ".bmp";
                strcat(filenameInput,add);
                char *fileOutput = strdup(argv[3]);
                strcat(fileOutput,add);
                printf("\n %s %s ",filenameInput,fileOutput );
                colorMap; //= memoryLock(colorMap,theImage->bmiHeader.biWidth,theImage->bmiHeader.biHeight);

                colorMap =  ReadBMP(filenameInput,theImage,header);
            
                writeMirrorFile(fileOutput,colorMap,theImage);
                free(colorMap);
            }else if(strstr(first,"-fh"))
            {
                char *filenameInput = strdup(argv[2]);
                char *add = ".bmp";
                strcat(filenameInput,add);
                char *fileOutput = strdup(argv[3]);
                strcat(fileOutput,add);
                printf("\n %s %s ",filenameInput,fileOutput );
                colorMap; //= memoryLock(colorMap,theImage->bmiHeader.biWidth,theImage->bmiHeader.biHeight);

                colorMap =  ReadBMP(filenameInput,theImage,header);
            
                writeMirrorHorizontalyFile(fileOutput,colorMap,theImage);
                free(colorMap);
            }else if(strstr(first,"-p"))
            {
                char *p = &first[2];
                int number = atoi(p);
                char *filenameInput = strdup(argv[2]);
                char *add = ".bmp";
                strcat(filenameInput,add);
                char *fileOutput = strdup(argv[3]);
                strcat(fileOutput,add);
                printf("\n %s %s ",filenameInput,fileOutput );
                colorMap; //= memoryLock(colorMap,theImage->bmiHeader.biWidth,theImage->bmiHeader.biHeight);
                colorMap =  ReadBMP(filenameInput,theImage,header);
                writeReducedFile(fileOutput,colorMap,theImage,number);
                free(colorMap);
            }
            
            
            
        }
    
    
    

    

    
    
    
    free(theImage);
    free(header);


    return 0;
    
}



struct rgb** ReadBMP(char* filename,struct bmp_info *theImage,struct bmp_header *header)
{
    
    int i,j;
    FILE* f = fopen(filename, "rb");
    if(f == NULL)
        return NULL;
    readHeader(f,header);
    readInfo(f,theImage);
    
    int width = theImage->bmiHeader.biWidth;

    int height = theImage->bmiHeader.biHeight;

    while (width % 4 != 0)
    {
        width++;
    }
    
    while (height % 4 != 0)
    {
        height++;
    }
    
    
    int row_padded = (width*3 + 3) & (~3);

    unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char)*row_padded);

    unsigned char tmp;

    struct rgb **some = memoryLock(some,width,height);
    
    for( i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for( j = 0; j < width*3; j += 3)
        {
            
            if (i > theImage->bmiHeader.biHeight && j > theImage->bmiHeader.biWidth*3 )
            {
                
                some[i][j/3].rgbBlue = 255;
                some[i][j/3].rgbGreen = 100;
                some[i][j/3].rgbRed = 90;
            }else
            {
             
            some[i][j/3].rgbBlue = (int)data[j+2];
            some[i][j/3].rgbGreen = (int)data[j+1];
            some[i][j/3].rgbRed = (int)data[j];
            }
            
            
            
            
        }
    }
    free(data);

    fclose(f);

    return some;
}

void printRgb(struct rgb **a){
    int i,j;
    for ( i = 0; i < 489; i++)
    {
        for (j = 0; j < 736; j++)
        {
            printf("\n %d : %d : %d ",a[i][j].rgbBlue,a[i][j].rgbRed,a[i][j].rgbGreen);
        }
        
    }
    
}

struct rgb** memoryLock(struct rgb** theImage,int width,int height)
{
    int i;
    theImage = (struct rgb **)malloc(sizeof(struct rgb *) * height);
    for (i = 0; i < height; i++)
    {
        theImage[i] = (struct rgb *)malloc(sizeof(struct rgb ) * width);
    }

    return theImage;
}

void writeFile(char *name,struct rgb** some,struct bmp_info *info)
{
    
    int i,j;
    FILE *writer = fopen(name,"wb");
    writeHeader(writer,info);
    writeInfo(writer,info);
    int width = info->bmiHeader.biWidth;
    int height = info->bmiHeader.biHeight;
    while (width % 4 != 0)
    {
        width++;
    }
    
    while (height % 4 != 0)
    {
        height++;
    }


    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            char *blue = &some[i][j].rgbBlue;
            char *red = &some[i][j].rgbGreen;
            char *green = &some[i][j].rgbRed;
            fwrite(blue,1,1,writer);
            fwrite(red,1,1,writer);
            fwrite(green,1,1,writer);
        }
        
    }
}

void printImageInfo(char *name,struct bmp_info *theImage,struct bmp_header *header)
{
    FILE *fp = fopen(name,"rb");
    if (fp == NULL)
    {
        printf("\nThe file does not exist!");
        exit(1);
    }
    
    readHeader(fp,header);
    readInfo(fp,theImage);
    printf("\n Header Type : %d",header->bfType );
    printf("\n Image Size : %d",header->bfSize);
    printf("\n Reserved Bits 1 : %d",header->bfReserved1);
    printf("\n Reserved Bits 2 : %d",header->bfReserved2);
    printf("\n Off : %d",header->bfOffBits);

    printf("\n Size of info header : %d",theImage->bmiHeader.biSize );
    printf("\n Width of image : %d",theImage->bmiHeader.biWidth );
    printf("\n Height of image : %d",theImage->bmiHeader.biHeight );
    printf("\n Number of color planes : %d",theImage->bmiHeader.biWidth );
    printf("\n Number of bits per pixel : %d",theImage->bmiHeader.biPlanes );
    printf("\n Type of compression to use : %d",theImage->bmiHeader.biBitCount );
    printf("\n Size of image data  : %d",theImage->bmiHeader.biSizeImage );
    printf("\n X pixels per meter : %d",theImage->bmiHeader.biXPelsPerMeter );
    printf("\n Y pixels per meter : %d",theImage->bmiHeader.biYPelsPerMeter );
    printf("\n Number of colors used : %d",theImage->bmiHeader.biClrUsed );
    printf("\n Number of important colors : %d",theImage->bmiHeader.biClrImportant );
    
    fclose(fp);


    
    
}

void writeMirrorFile(char *name,struct rgb** some,struct bmp_info *info)
{
    int i,j;
    FILE *writer = fopen(name,"wb");
    writeHeader(writer,info);
    writeInfo(writer,info);
    int width,height;
    width = info->bmiHeader.biWidth;
    height = info->bmiHeader.biHeight;
    while (width % 4 != 0)
    {
        width++;
    }
    
    while (height % 4 != 0)
    {
        height++;
    }
    
    for (i = 0; i < height; i++)
    {
        for (j = width-1; j >=0; j--)
        {
            if (i > info->bmiHeader.biHeight || j < (width) - info->bmiHeader.biWidth)
            {
                char *blue = "0";
                char *red = "0";
                char *green = "0";
                fwrite(blue,1,1,writer);
                fwrite(red,1,1,writer);
                fwrite(green,1,1,writer);
            }else{
                char *blue = &some[i][j].rgbBlue;
                char *red = &some[i][j].rgbGreen;
                char *green = &some[i][j].rgbRed;
                fwrite(blue,1,1,writer);
                fwrite(red,1,1,writer);
                fwrite(green,1,1,writer);
            }
            
            
        }
        
    }
    fclose(writer);
}

void writeMirrorHorizontalyFile(char *name,struct rgb** some,struct bmp_info *info)
{
    int i,j;
    FILE *writer = fopen(name,"wb");
    writeHeader(writer,info);
    writeInfo(writer,info);
    int width,height;
    width = info->bmiHeader.biWidth;
    height = info->bmiHeader.biHeight;
    while (width % 4 != 0)
    {
        width++;
    }
    
    while (height % 4 != 0)
    {
        height++;
    }
    
    for (i = height -1; i >=0; i--)
    {
        for (j = 0; j < width; j++)
        {
            
                
                char *blue = &some[i][j].rgbBlue;
                char *red = &some[i][j].rgbGreen;
                char *green = &some[i][j].rgbRed;
                

                
                fwrite(blue,1,1,writer);
                fwrite(red,1,1,writer);
                fwrite(green,1,1,writer);
            
            
            
        }
        
    }
    fclose(writer);
}





void writeReducedFile(char *name,struct rgb** some,struct bmp_info *info,int numb)
{
    int i,j;
    if (numb % 2 != 0 || numb > 256)
    {
        printf("\nThe number must be divided from zero!");
        exit(1);
    }
    
    int **split =(int**)malloc(sizeof(int*) * numb);
    if (split == NULL)
    {
        printf("\nMemory Allocation Failed");
        exit(1);
    }
    
    for (i = 0; i < numb; i++)
    {
        split[i] = (int*)malloc(sizeof(int) * 256/numb);
        if (split[i] == NULL)
        {
            printf("\nMemory Allocation Failed");
            exit(1);
        }
        
    }
    
    for (i = 0; i < numb; i++)
    {
        
        for (j = (256*i)/numb; j <(i+1)*(256/numb); j++)
        {
            split[i][j - i*(256/numb)] = j;
            
        }
        
    }
    
    

    FILE *writer = fopen(name,"wb");

    writeHeader(writer,info);

    writeInfo(writer,info);

    int width,height;

    width = info->bmiHeader.biWidth;

    height = info->bmiHeader.biHeight;

    while (width % 4 != 0)
    {
        width++;
    }
    
    while (height % 4 != 0)
    {
        height++;
    }
    int k,l;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            
                
                char *blue = &some[i][j].rgbBlue;
                
                
                for (k = 0; k < numb; k++)
                {
                    for ( l = 0; l < 256/numb; l++)
                    {
                        if (*blue == split[k][l])
                        {
                            *blue = split[k][0];
                            
                        }
                    }
                }
                
                

                
                char *red = &some[i][j].rgbGreen;
                
                for (k = 0; k < numb; k++)
                {
                    for ( l = 0; l < 256/numb; l++)
                    {
                        if (*red == split[k][l])
                        {
                            *red = split[k][0];
                            
                        }
                    }
                   
                    
                }
                char *green = &some[i][j].rgbRed;
                
                
                
                for (k = 0; k < numb; k++)
                {
                    for ( l = 0; l < 256/numb; l++)
                    {
                        if (*green == split[k][l])
                        {
                            *green = split[k][0];
                            
                        }
                    }
                   
                    
                }
                fwrite(blue,1,1,writer);
                fwrite(red,1,1,writer);
                fwrite(green,1,1,writer);
            
            
            
        }
        
    }
    fclose(writer);
}
void writePercentageFile(char *name,struct rgb** some,struct bmp_info *info,int numb)
{
    int i,j;

   

    FILE *writer = fopen(name,"wb");
    writeHeader(writer,info);
    writeInfo(writer,info);
    int width,height;
    width = info->bmiHeader.biWidth;
    height = info->bmiHeader.biHeight;
    while (width % 4 != 0)
    {
        width++;
    }
    
    while (height % 4 != 0)
    {
        height++;
    }
    
    for (i = 0; i < height; i++)
    {
        for (j = width-1; j >=0; j--)
        {
            if (i > info->bmiHeader.biHeight || j < (width) - info->bmiHeader.biWidth)
            {
                char *blue = "0";
                char *red = "0";
                char *green = "0";
                fwrite(blue,1,1,writer);
                fwrite(red,1,1,writer);
                fwrite(green,1,1,writer);
            }else{
                char *blue = &some[i][j].rgbBlue;
                int x = atoi(blue);

                
                char *red = &some[i][j].rgbGreen;
                int y = atoi(red);
                
                
                char *green = &some[i][j].rgbRed;
                int z = atoi(green);
                int comp = FindNearestColor2(red,blue,green,info);
                if (comp == 0 )
                {
                    fwrite(blue,1,1,writer);
                    fwrite(red,1,1,writer);
                    fwrite(green,1,1,writer);
                }else
                {
                    
                    *blue = ((*blue) + (*red) + (*green))/3;
                    *red = ((*blue) + (*red) + (*green))/3;
                    *green = ((*blue) + (*red) + (*green))/3;
                    fwrite(blue,1,1,writer);
                    fwrite(red,1,1,writer);
                    fwrite(green,1,1,writer);

                }
                
                
            }
            
            
        }
        
    }
    fclose(writer);
}


int FindNearestColor(struct rgb **more, struct bmp_info *header)
{

    int i,j;
    struct rgb compare;
    compare.rgbBlue = 0;
    compare.rgbRed = 255;
    compare.rgbGreen = 0;
    int size = header->bmiHeader.biWidth * header->bmiHeader.biHeight;
    int percentage = size * 0.1;
    int *arr = (int*)malloc(sizeof(int) * size);
    
    //struct rgb *compare[percentage];
    for ( i = 0; i < header->bmiHeader.biHeight; i++)
    {
        for ( j = 0; j < header->bmiHeader.biWidth; j++)
        {
            //int rmean = ( (more[i][j].rgbRed +  compare.rgbRed )) / 2;
            int r = more[i][j].rgbRed -  compare.rgbRed;
            int g = more[i][j].rgbGreen -  compare.rgbGreen;
            int b = more[i][j].rgbBlue -  compare.rgbBlue;
            // long d =sqrt((r)^2+(g)^2+(b)^2);
            // long p = d/sqrt((255)^2+(255)^2+(255)^2);
            int p = r*r + g*g + b*b;
            //int p =  sqrt((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8));
            arr[i+j] = p;
            
            
        }
        
    }
    mergeSort(arr,0,size-1);
    int *arr2 = (int*)malloc(sizeof(int) * percentage);
    
    j = 0;
    for (i =size-1; i > size-percentage ; i--)
    {
        if (arr[i] == 0)
        {
            break;
        }
        
        arr2[j] = arr[i];
        j++;
        
        
    }
    
    
    printArray(arr2,j);
    
  return 1;
  
}


int FindNearestColor2(char *red ,char *blue , char *green, struct bmp_info *header)
{

    int i,j;
    struct rgb compare;
    compare.rgbBlue = 0;
    compare.rgbRed = 255;
    compare.rgbGreen = 0;
    //int size = header->bmiHeader.biWidth * header->bmiHeader.biHeight;
    //int percentage = size * 0.1;
    //int *arr = (int*)malloc(sizeof(int) * size);
    
    //struct rgb *compare[percentage];
    
            //int rmean = ( (more[i][j].rgbRed +  compare.rgbRed )) / 2;
            int r = *red -  compare.rgbRed;
            int g = *green -  compare.rgbGreen;
            int b = *blue -  compare.rgbBlue;
            // long d =sqrt((r)^2+(g)^2+(b)^2);
            // long p = d/sqrt((255)^2+(255)^2+(255)^2);
            int p = (r*r) + (g*g) + (b*b);
            
            
            if (p == 0)
            {
                return 0;
            }else{
                return 1;
            }
            
            

}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    
    
    int *L = (int*)malloc(sizeof(int) * n1);
    int *R = (int*)malloc(sizeof(int) * n1);
    
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 

void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
       
        int m = l + (r - l) / 2;
 
        
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
 
        merge(arr, l, m, r);
    }
}
 

