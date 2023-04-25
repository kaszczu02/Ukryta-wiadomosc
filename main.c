#include <stdio.h>
#include <string.h>

union bit_reader
{
    struct
    {
        unsigned char bit1 : 1;
        unsigned char bit2 : 1;
        unsigned char bit3 : 1;
        unsigned char bit4 : 1;
        unsigned char bit5 : 1;
        unsigned char bit6 : 1;
        unsigned char bit7 : 1;
        unsigned char bit8 : 1;
    }unia;
    unsigned char slowo;
};


int decode(const char * filename, char *txt, int size)
{
    if(filename == NULL || txt == NULL || size <= 0)
    {
        return 1;
    }
    FILE *file = NULL;
    union bit_reader odczyt,zapis;

    file = fopen(filename,"r");
    if(file == NULL)
    {
        return 2;
    }
    int tym;
    int size_filename = 0;

    while(!feof(file))
    {
        int x = fscanf(file,"%d",&tym);
        if(x != -1)
        {
            size_filename++;
        }
        if(x == 0)
        {
            fclose(file);
            return 3;
        }
        if(tym < 0 || tym > 255)
        {
            fclose(file);
            return 3;
        }



    }
    rewind(file);

    if(((size_filename % 8) != 0) && ((size * 8) > size_filename))
    {
        fclose(file);
        return 3;
    }



    int index = 0;
    while(!feof(file))
    {
        for(int i = 0; i < 8; i++)
        {
            fscanf(file,"%hhu",&odczyt.slowo);
            if(i == 0)
            {
                zapis.unia.bit8 = odczyt.unia.bit1;
            }
            else if(i == 1)
            {
                zapis.unia.bit7 = odczyt.unia.bit1;
            }
            else if(i == 2)
            {
                zapis.unia.bit6 = odczyt.unia.bit1;
            }
            else if(i == 3)
            {
                zapis.unia.bit5 = odczyt.unia.bit1;
            }
            else if(i == 4)
            {
                zapis.unia.bit4 = odczyt.unia.bit1;
            }
            else if(i == 5)
            {
                zapis.unia.bit3 = odczyt.unia.bit1;
            }
            else if(i == 6)
            {
                zapis.unia.bit2 = odczyt.unia.bit1;
            }
            else if(i == 7)
            {
                zapis.unia.bit1 = odczyt.unia.bit1;
            }

        }
        *(txt + index) = (char)zapis.slowo;
        index = index + 1;
        if(index == size - 1)
        {
            break;
        }

    }
    *(txt + index) = '\0';
    fclose(file);
    return 0;
}

int encode(const char *input, char *txt, const char *output)
{
    if(input == NULL || txt == NULL || output == NULL)
    {
        return 1;
    }

    FILE *file_input = NULL;
    FILE *file_output = NULL;
    union bit_reader odczyt,zapis;
    file_input = fopen(input,"r");
    if(file_input == NULL)
    {
        return 2;
    }
    file_output = fopen(output,"w");
    if(file_output == NULL)
    {
        fclose(file_input);
        return 4;
    }

    int tym;
    int size_input = 0;
    while(!feof(file_input))
    {
        if(fscanf(file_input,"%d",&tym) == 0)
        {
            fclose(file_input);
            fclose(file_output);
            return 3;
        }
        if(tym < 0 || tym > 255)
        {
            fclose(file_input);
            fclose(file_output);
            return 3;
        }
        size_input++;
    }

    rewind(file_input);

    size_input = size_input - 1;
    int size_txt = strlen(txt);
    if(((int)size_txt * 8 > size_input))
    {
        fclose(file_input);
        fclose(file_output);
        return 3;
    }

    char x;
    for(int i = 0; i < (int)strlen(txt); i++)
    {
        odczyt.slowo = *(txt + i) ;
        int j = 0;
       while(j != 8)
        {
           fscanf(file_input,"%c",&x);
            if(x >= '0' && x <= '9')
            {
                fseek(file_input,-1L,SEEK_CUR);
                fscanf(file_input,"%hhu",&zapis.slowo);
                if(j == 0)
                {
                    zapis.unia.bit1 = odczyt.unia.bit8;
                }
                else if(j == 1)
                {
                    zapis.unia.bit1 = odczyt.unia.bit7;
                }
                else if(j == 2)
                {
                    zapis.unia.bit1 = odczyt.unia.bit6;
                }
                else if(j == 3)
                {
                    zapis.unia.bit1 = odczyt.unia.bit5;
                }
                else if(j == 4)
                {
                    zapis.unia.bit1 = odczyt.unia.bit4;
                }
                else if(j == 5)
                {
                    zapis.unia.bit1 = odczyt.unia.bit3;
                }
                else if(j == 6)
                {
                    zapis.unia.bit1 = odczyt.unia.bit2;
                }
                else if(j == 7)
                {
                    zapis.unia.bit1 = odczyt.unia.bit1;
                }
                fprintf(file_output,"%hhu ",zapis.slowo);
                j++;


            }
            else if(x == '\n')
            {
                fprintf(file_output,"\n");

                continue;
            }

        }

    }

    while(!feof(file_input))
    {
        fscanf(file_input,"%c",&x);
        if(x >= '0' && x <= '9')
        {
            fseek(file_input,-1L,SEEK_CUR);
            fscanf(file_input,"%hhu",&odczyt.slowo);
            odczyt.unia.bit1 = 0;
            fprintf(file_output,"%hhu ",odczyt.slowo);
        }
        else if(x == '\n')
        {
           fprintf(file_output,"\n");
        }

    }
    fclose(file_input);
    fclose(file_output);
    return 0;

}




int main()
{
    char tab[1001];
    char input[30];
    char output[30];
    int e,d;
    printf("Do you want to encode (E/e) or decode (D/d) a message? ");
    char przycisk = getchar();
    scanf("%*[^\n]s");
    scanf("%*c");
    if(przycisk == 'E' || przycisk == 'e')
    {
        printf("Enter a message to be encoded ");
        scanf("%1000[^\n]",tab);
        scanf("%*[^\n]s");
        scanf("%*c");
        printf("Enter input file name: ");
        scanf("%29[^\n]",input);
        scanf("%*[^\n]s");
        scanf("%*c");
        printf("Enter output file name: ");
        scanf("%29[^\n]",output);
        scanf("%*[^\n]s");
        scanf("%*c");
        e = encode(input,tab,output);
        if(e == 2)
        {
            printf("Couldn't open file");
            return 4;
        }
        if(e == 4)
        {
            printf("Couldn't create file");
            return 5;
        }
        if(e == 3)
        {
            printf("File corrupted");
            return 6;
        }
        printf("File saved");


    }
    else if(przycisk == 'D' || przycisk == 'd')
    {
        printf("Enter input file name: ");
        scanf("%29[^\n]",input);
        scanf("%*[^\n]s");
        scanf("%*c");
        d = decode(input,tab,sizeof(tab));
        if(d == 2)
        {
            printf("Couldn't open file");
            return 4;
        }
        if(d == 3)
        {
            printf("File corrupted");
            return 6;
        }
        printf("%s",tab);


    }
    else
    {
        printf("Incorrect input data");
        return 1;
    }
    return 0;
}


