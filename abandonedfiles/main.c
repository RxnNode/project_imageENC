/*
 * Date : 2020.06.03
 * Author: Yung-An Jen
 * C standard : c11
 * LLVM version: 10.0.1 (clang-1001.0.46.4)
 */

#include <stdio.h>
#include <stdlib.h>
#define INT_BIT 2
#define buffersize 8


unsigned int offset = 0;



int enc(const char *picname, const char *hfile){

    FILE *imgr = NULL;
    FILE *fout = NULL;
    FILE *enct = NULL;

    char bmp[2] = {};
    int size = 0;
    unsigned char header[offset];


    int infosize = 0;
    int bits = 0;

    /* 
     * Sourcing image file
     */
    imgr = fopen(picname,"rb+");
    // error handling
    if (imgr == NULL)
    {
        printf("Failed to read image file.");
        return -1;
    }else
    {
        fseek(imgr, 10, SEEK_SET);  //Set offset
        fread(&offset, sizeof(int), 1, imgr);   //Get offset
        
    }
    
    
    /* 
     * print image information
     */
    if (imgr) {
        printf("===============Image Information============\n");
        fread(bmp, sizeof(char), 2, imgr);
        printf("File Type: %s\n", bmp);

        fread(&size, sizeof(int), 1, imgr);
        printf("File Size: %d byte\n", size);

        // fseek(imgr, 10, SEEK_SET);
        // fread(&offset, sizeof(int), 1, imgr);
        printf("Pixel start from: %d\n", offset);

        fread(&infosize, sizeof(int), 1, imgr);
        printf("Info Size: %d\n", infosize);

        fseek(imgr, 10, SEEK_CUR);
        fread(&bits, 2, 1, imgr);
        printf("Bits per pixel: %d\n", bits);
        printf("============================================\n");
    } else{
        printf("Can't read file.\n");
        return -1;
    }

    /*
     *  Encryption start
     */

        

        fseek(imgr, offset, SEEK_SET);  // reset source image to offset
        
        // Create output bmp file
        char oFileBuff[32];
        char oFileName[32];
        // set the output file name 
        printf("Please enter output file name(alphabetic, max 32 chars): ");
        if(scanf("%32s",oFileBuff) != EOF){
            snprintf(oFileName, sizeof(oFileName) ,"%s.bmp", oFileBuff);
            printf("Your output file name: %s\n",oFileName);
            printf("============================================\n");
        } else{
            printf("Failed to create output file.\n");
            printf("============================================\n");
            return -1;
        }
        // make output file
        fout = fopen(oFileName,"wb+");
        if (fout == NULL){
            printf("Can't create output file.\n");
            printf("============================================\n");
            return -1;
        } else{
            // Read bmp header
            fseek(imgr, 0, SEEK_SET);
            fread(&header, sizeof(char), offset, imgr);
            // Write header into Output file.
            fwrite(header, sizeof(unsigned char), offset, fout);
            printf("Bmp header writing...\n");
        }

        //  Read enc text file
        enct = fopen(hfile,"rb");
        fseek(enct, 0, SEEK_SET);   // reset text file to start

        if (enct == NULL){
            printf("Can't not read text file.\n");
            printf("============================================\n");
            return -1;
        } else{
            

            unsigned int c, enc, mask = 0b11111110;

            while ((enc = (unsigned)fgetc(enct)) != EOF){
                for (int i = 7; i >= 0; i--) {
                    if ((c = (unsigned) fgetc(imgr)) != EOF) {
                        c = (c & mask) | ((enc & (1 << i)) >> i);
                        fputc(c, fout);
                    } else {
                        printf("This file is too small too hide all text file.\n");
                        printf("============================================\n");
                        return 1;
                    }
                }
            }
            printf("Writing ...\n");
            // Add rest of pixels
            while ((c = (unsigned)fgetc(imgr)) != EOF){
                fputc(c,fout);
            }
        }
        fclose(imgr);
        fclose(enct);
        fclose(fout);
        fflush(stdin);
        printf("Done!\n");
    printf("============================================\n");
    return 0;

}

int dec(const char *dpic){

    /*
     *  Decryption
     *  Output file : dec.text
     *
     */

    FILE *dec = NULL;
    FILE *fout = NULL;

    fout = fopen(dpic,"rb+");
    if ( fout == NULL){
        printf("Can't find output file. Decryption terminated.\n");
        printf("============================================\n");
        return -1;
    } else{

        fseek(fout, 10, SEEK_SET);
        fread(&offset, sizeof(int), 1, fout);
        
        dec = fopen("dec.txt","wb+");
        if (dec == NULL){
            printf("Can't create decrypted file.\n");
            printf("============================================\n");
            return -1;
        } else{
            unsigned int oc, dc = 0b00000000;
            fseek(fout, offset, SEEK_SET);
            while((oc = (unsigned)fgetc(fout)) != EOF){

                oc = oc & 1;
                dc = dc | (oc << 7);
                for (int i = 6; i >= 0 ; i--) {
                    oc = (unsigned)fgetc(fout);
                    oc = oc & 1;
                    dc = dc | (oc << i);
                }
                fputc(dc, dec);
                dc = 0b00000000;
                oc = 0b00000000;

            }
        }
        printf("Decryption Done!\n");
        printf("============================================\n");
    }
}

int main() {

    char s = '0';

    printf("==============Image En/Decryption===========\n");
    printf("*bmp file only*\n");
    printf("1. Enc 2. Dec?: ");

    while (scanf("%1c",&s)){
        char path[256];
        char fpath[256];

        if (s == '1'){
            printf("============================================\n");
            printf("Please enter image(bmp) full path & file name(alphabetic): ");

            if( scanf("%255s",path) != EOF){
                printf("Your image file: %s\n", path);
                printf("============================================\n");
                printf("Please enter full path & file name(alphabetic) of file: ");

                if(scanf("%255s",fpath) != EOF){
                    printf("Your image file: %s\n",fpath);
                    printf("============================================\n");
                } else{
                    printf("Failed to read file.\n");
                    printf("============================================\n");
                    return -1;
                }
            } else{
                printf("Failed to read image file.\n");
                printf("============================================\n");
                return -1;
            }

            enc(path,fpath);

        } else if ( s == '2'){
            printf("============================================\n");
            printf("Please enter Encrypted image(bmp) full path & file name(alphabetic): ");
            if(scanf("%255s",path) != EOF){
                printf("Your Encrypted image file: %s\n", path);
                printf("============================================\n");
            } else{
                printf("Failed to read image file.\n");
                printf("============================================\n");
                return -1;
            }

            dec(path);

        } else{
            printf("only 1 or 2\n");
            printf("============================================\n");
        }
        s = '0';

        return 0;
    }



/*
    FILE *dir, *dir2;
    FILE *out;
    FILE *outtest;
    int c,count=0, show=0;
    Boolean test = false;

    dir = fopen("../images/image.jpg","r+b");
    //dir = fopen("../images/parrots.jpeg","r+b");

    if (dir== NULL){
        printf("Can't read file.");
        perror("Error");
    } else{
        printf("Read image successfully.\n");
        out = fopen("../out.txt","wb+");
        if (out){
            while ((c=fgetc(dir))!=EOF) {
                if(show <= 16 )printf("%d ",c);
                show++;
                int binbuf[buffersize];
                binaryn(binbuf, c);
                //if (show != 10) {
                    for (int i = 0; i < buffersize; ++i) {
                        fputc(binbuf[i], out);
                        //printf("%d", binbuf[i]);
                    }
                    //show++;
                    //printf(" ");
                //}
                count++;
            }
            printf("\n%d byte read.\n",count);
            printf("Create a output file successfully.\n");
            test = true;
        } else{
            printf("Can't create a output file.\n");
            //perror("Error");
        }
        fclose(dir);
        fclose(out);
    }
*/

/*
    if (test){

        FILE *cmess;
        cmess = fopen("../Enc/Enc/secret","rb+");
        if (cmess){
            printf("Read message successfully.\n");

        } else{
            printf("Can't read message file.\n");
            //perror("Error");
        }

    } else{

    }
*/



/*
 *  output file check
 *  If it can be converted back to original jpg that binary algorithm is correct.
 *  Yes! It works!
 *
    FILE *outback;
    show = 0;
    outtest = fopen("../out.txt","rb");
    outback = fopen("../out.jpg","wb+");
    if (outtest == NULL){
        perror("Error");
    } else{
        printf("\nStart checking output file...\n");
        int obitc = 8, p = 0b00000000;
        int buf[obitc];
        while ((c=fgetc(outtest))!=EOF){

            buf[obitc-1] = c;

            //if (show <=31) printf("%d",c);
            //show++;

            if (obitc == 1){
                obitc = 8;
                for (int i = 0; i < 8; ++i) {

                    if (show <=31)printf("%d",buf[i]);
                    show++;

                    if (buf[i]!=0){
                        p = p | (1 << i);
                    }
                }
                //if (show <=16) printf("%d ",p);
                printf(" "),
                //show ++;
                fputc(p,outback);
                p = 0b00000000;

            } else{
                obitc--;
            }

        }

    }
    fclose(outback);
    fclose(outtest);
*/

/*
 *  binary convert test
 *
    int bin[buffersize], p=0b00000000;
    int inp =90;
    printf("\n");
    printf("Binary: ");
    char b[10]="0b00001001";
    binaryn(bin,inp);

    for (int i = 0; i < buffersize; ++i){
        printf("%d", bin[i]);
        if (bin[i]!=0){
            p = p | (1 << (7-i));
        }
    }

    printf("\nString: %s\n",b);
    printf("pa: %p\n",&p);
    printf("pv: %d\n",p);
*/


/*
 *  << test
 *
    printf("\n%d",1<<0);
    printf("%d",1<<1);
    printf("%d",1<<2);
    printf("%d",1<<3);
    printf("\nSP : %d",(12 & (1<<0)));
    printf("\nSP : %d",(12 & (1<<1)));
    printf("\nSP : %d",(12 & (1<<2)));
    printf("\nSP : %d",(12 & (1<<3)));

*/

/*
 * abandoned binary algorithm
 *
    long temp, a = 43;

    int Arr[64] = {};

    for (int i = 0; i < 64; i++){
        temp = a % 2;
        if ( temp != 0){
            Arr[i] = 1;
        } else{
            Arr[i] = 0;
        }
        if (a != 0) a = a/2;
    }

*/

/*
 * Array copying practice
 *
    int *s[64];

    printf("Srr address: %p\n", s);
    printf("Arr address: %p\n", Arr);

    for (int k = 0; k < 64; ++k) {
        s[k] = &Arr[k];

    }

    printf("Size of S: %d\n", (int)(sizeof(s)/sizeof(s[0])));
    printf("Size of Arr: %d\n", (int)(sizeof(Arr)/sizeof(Arr[0])));

    printf("Arr :");
    for (int j = 0; j < (int)(sizeof(Arr)/sizeof(Arr[0])); ++j) {

        printf("%d", Arr[j]);
    }

    printf("\nSrr :");

    for (int j = 0; j < (int)(sizeof(s)/sizeof(s[0])); ++j) {

        printf("%d", *s[j]);
    }
*/



    return 0;
}