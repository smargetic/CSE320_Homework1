#include "const.h"
#include "transplant.h"
#include "debug.h"
//#include "const2.h"


#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

//int countPath_buf;
char* nameBytes(char* str1);
void directoryEntry(char* str1, int depth, long int size);
int serializeEndDirectory(int depth);
void serializeDepth(int depth);
void serializeSize(long int size);
int getLength(char* str1);
int serializeStartDirectory(int depth);
void filesRecursively(char *base, int depth);

int deserializeDepth(int b1, int b2, int b3, int b4);
//int deserializeDepth(char temp1, char temp2, char temp3, char temp4);
long deserializeSize(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8);
int deserializeStartTransmission(int depth);
int deserializeDirectoryEntry(int depth);
int deserializeEndDirectory(int depth);
int deserializeEndTransmission(int depth);
//int hexToInt(char val);
//int lengthD(char* arr);
//char* getPoint();
//int origDepth;
int nameCount =0;
int tickEntry;

//char** doublePoint;

int lenghtBigArray;
int fileLocation;
long sizeDirEntry;
/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the pathname of the current file or directory
 * as well as other data have been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */
int stringCompare(char *str1, char *str2){
    //IF EQUAL --> RETURNS 0
    int truth =0;
    int it = 0;
    //if either of the strings are null, I throw an error
    //if ((str1 ==NULL)||(str2==NULL)) {
    //    return -1;
    //}
    if((*str1=='\0')||(*str2=='\0')){
        return -1;
    }

    while (truth ==0){
        //I increment indexes of strings and compare values
        if(*(str1+it) != *(str2+it)){
            //if the values are not equal, I throw an error

            return -1;
        }
        else if((*(str1+it)=='\0')&&(*(str2+it)=='\0')){
            return 0;
        }
        it++;

    }
    return -1;
}
/*
char* getPoint(){
    char* ptr;
    return ptr;
}
*/
int lengthD(char* arr){
    int count =0;
    while(*arr!='\0'){
        count++;
    }
    return count;
}


int getDeserializedDepth(){
    int depth;
    int b1 = getchar();
    int b2 = getchar();
    int b3 = getchar();
    int b4 = getchar();
    depth = deserializeDepth(b1,b2,b3,b4);
    return depth;
}
long getDeserializedSize(){
    long size;
    int b1 = getchar();
    int b2 = getchar();
    int b3 = getchar();
    int b4 = getchar();
    int b5 = getchar();
    int b6 = getchar();
    int b7 = getchar();
    int b8 = getchar();

    size = deserializeSize(b1,b2,b3,b4,b5,b6,b7,b8);
    return size;
}

int power(int base, int power){
    int num =1;
    if (power ==0){
        return 1;
    }
    for(int i=1; i<=power; i++){
        num = base*num;
    }
    return num;
}

int validPath(char *str1){
    //NEED TO FIGURE OUT
    DIR* mydir = opendir(str1);
    if(mydir==NULL){
        return -1;
    }
    return 0;
}

int hexToInt(char val){
    int intVal;
    if((val>='a')&&(val<='f')){
        intVal = val-'a' +10;
    } else if ((val>='A')&&(val<='F')){
        intVal = val - 'A' +10;
    } else if((val>='0')&&(val<='9')){
        intVal = val -'0';
    }
    return intVal;

}


char** passingthrough(char** doublePoint){
    return doublePoint;
}

void filesRecursively(char *base, int depth){

    //I open the directory given

    struct dirent *dp;
    DIR *dir = opendir(base);
    char* tempName;


    depth++;

    while((dp=readdir(dir))!=NULL){
        //I read to see if there are any subdirectories
        //If there is, I push it onto path_buf
        tempName = dp->d_name;


        if((stringCompare(tempName, ".")!=0)&&(stringCompare(tempName,"..")!=0)){
            path_push(dp->d_name);

            //certain functions require a pointer to be passed

            char *ptrName;
            ptrName = path_buf;

            struct stat ptrName_stat;
            stat(ptrName, &ptrName_stat);

            //I find the size to be used later
            long int size =ptrName_stat.st_size;

            int len = getLength(tempName);

            long int directoryEntrySize = 12 + 16 + len;


            //I call on directory entry --> because it is an entry
            directoryEntry(tempName, depth, directoryEntrySize);
            //if it is a file, I try to read it
            if(S_ISREG(ptrName_stat.st_mode)!=0){
                serialize_file(depth, size);

                //if it is a file, there is nothing bellow it, so I pop the name
                path_pop();
            }
            else if(S_ISDIR(ptrName_stat.st_mode)!=0){
                //then check if directory

                //if it is a directory, I serialize it
                serializeStartDirectory(depth+1);

                //I check if it has sub folders
                filesRecursively(ptrName, depth);

                serializeEndDirectory(depth+1);

            } else {
                //neither file or a directory
                //I pop the value
                path_pop();
            }

        }
    }
    path_pop();
    closedir(dir);
    return;
}

int getLength(char* str1){
    int count=0;
    while(*str1!='\0'){
        count++;
        str1++;
    }
    return count;

}

int serializeEndDirectory(int depth){
    //magic bytes
    putchar(0x0C);
    putchar(0x0D);
    putchar(0xED);
    putchar(0x03);
    serializeDepth(depth);
    long int size = 16;
    serializeSize(size);
    return 0;
}


int findSize(char* str1){
    int size =0;
    //in order to find the size, I have to open the file and count the number of bytes
    FILE *fptr;
    fptr = fopen(path_buf, "r");
    int charTemp;
    //I iterate through the file and output it in hex
    while((charTemp=getc(fptr)) != EOF){
        size++;

    }

    fclose(fptr);
    return size;
}




void serializeDepth(int depth){
    //I create the depth 4 bytes
    char depth1;
    char depth2;
    char depth3;
    char depth4;

    //I take the last 8 binary terms
    depth4 = depth &0xFF;
    //next group
    depth3 = (depth>>8) & 0xFF;
    depth2 = (depth>>16) & 0xFF;
    depth1 = (depth>>24) & 0xFF;

    putchar(depth1);
    putchar(depth2);
    putchar(depth3);
    putchar(depth4);
}

void serializeSize(long int size){
    char b1;
    char b2;
    char b3;
    char b4;
    char b5;
    char b6;
    char b7;
    char b8;

    //I take the last 8 binary terms
    b8 = size &0xFF;
    //next group
    b7 = (size>>8) & 0xFF;
    b6 = (size>>16) & 0xFF;
    b5 = (size>>24) & 0xFF;
    b4 = (size>>32) &0xFF;
    //next group
    b3 = (size>>40) & 0xFF;
    b2 = (size>>48) & 0xFF;
    b1 = (size>>54) & 0xFF;

    putchar(b1);
    putchar(b2);
    putchar(b3);
    putchar(b4);
    putchar(b5);
    putchar(b6);
    putchar(b7);
    putchar(b8);
}

int serializeStartDirectory(int depth) {
    putchar(0x0C);
    putchar(0x0D);
    putchar(0xED);
    putchar(0x2);
    serializeDepth(depth);
    int size = 16;
    serializeSize(size);

    return 0;
}


void directoryEntry(char* str1, int depth, long int size){
    char *ptrName;
    ptrName = path_buf;

    char* placeHolder;
    char* placeHolder2;
    int count =0;
    int count2 =0;
    int charValue = 0;

    //magic bytes
    putchar(0x0C);
    putchar(0x0D);
    putchar(0xED);
    //type
    putchar(0x4);
    serializeDepth(depth);
    serializeSize(size);

    struct stat ptrName_stat;
    stat(ptrName, &ptrName_stat);
    //I get the mode
    int mode = ptrName_stat.st_mode;
    serializeDepth(mode);
    //I get the size of the file
    long int sizeContents = ptrName_stat.st_size;
    serializeSize(sizeContents);

    while(*str1!='\0'){
        charValue = *str1;
        putchar(charValue);
        str1++;
    }




}


int deserializeDirectoryEntry(int depth){
    char* name;
    //deserialize depth
    int deDepth = getDeserializedDepth();
    if(deDepth!=depth){
        return -1;
    }

    //deserialize size
    long size = getDeserializedSize();
    size = size -12 -16; //length of a name

    int mode = getDeserializedDepth(); //DOUBLE CHECK THIS
    //I check the mode
    if(S_ISDIR(mode)!=0){ //not sure if I need to use a pointer
        tickEntry = 0;
        //I give some sort of tick --> to be returned to other function
    } else if (S_ISREG(mode)!=0){
        tickEntry = 1;
        //I give a different tick
    }


    //the next 8 bytes are the size
    sizeDirEntry = getDeserializedSize();

    int val;
    for(int i=0; i<size;i++){
        val = getchar();
        *name = val;
        name++;
    }

    path_push(name);//I add the name to the current path

    return 0;
}

int deserializeStartTransmission(int depth){
    //make sure the magic bytes are there
    int b1 =getchar();
    int b2 = getchar();
    int b3 = getchar();
    int b4 = getchar(); //type
    if((b1!=12)||(b2!=13)||(b3!=237)||(b4!=0)){
        return -1;
    }
    int deDepth = getDeserializedDepth();
    if (deDepth!=depth){
        return -1;
    }
    long size = getDeserializedSize();
    if(size!=16){
        return -1;
    }
    return 0;
}

int deserializeEndTransmission(int depth){

    int deDepth = getDeserializedDepth();
    if (deDepth!=depth){
        return -1;
    }
    long size = getDeserializedSize();
    if(size!=16){
        return -1;
    }

    int a = getchar();

    if(a!=EOF){
        return -1;
    }



    return 0;
}

int deserializeEndDirectory(int depth){
    //deserialize depth

    int deDepth = getDeserializedDepth();
    if(deDepth!=depth){
        return -1;
    }

    //deserialize size
    long size = getDeserializedDepth();

    if(size!=16){
        return -1;
    }

    path_pop();
    return 0;
}


/*
 * A function that returns printable names for the record types, for use in
 * generating debugging printout.
 */
static char *record_type_name(int i) {
    switch(i) {
    case START_OF_TRANSMISSION:
	return "START_OF_TRANSMISSION";
    case END_OF_TRANSMISSION:
	return "END_OF_TRANSMISSION";
    case START_OF_DIRECTORY:
	return "START_OF_DIRECTORY";
    case END_OF_DIRECTORY:
	return "END_OF_DIRECTORY";
    case DIRECTORY_ENTRY:
	return "DIRECTORY_ENTRY";
    case FILE_DATA:
	return "FILE_DATA";
    default:
	return "UNKNOWN";
    }
}

/*
 * @brief  Initialize path_buf to a specified base path.
 * @details  This function copies its null-terminated argument string into
 * path_buf, including its terminating null byte.
 * The function fails if the argument string, including the terminating
 * null byte, is longer than the size of path_buf.  The path_length variable
 * is set to the length of the string in path_buf, not including the terminating
 * null byte.
 *
 * @param  Pathname to be copied into path_buf.
 * @return 0 on success, -1 in case of error
 */
int path_init(char *name) {
    //int nameCount;
    char* temp = name;
    //if nothing is passed, I throw an error
    if(*temp == 0){
        return -1;
    }

    //if there is something in the path_buf already, I have to clear it out
    int valNameCount = nameCount;
    char* ptr = path_buf;
    if(valNameCount>0){
        for(int i=0;i<nameCount; i++){
            *ptr = '\0';
            ptr++;
        }
    }

    //I set the values of path_buf to be equal to the name passed
    ptr = path_buf;
    while(*name != 0){
        nameCount++;
        *ptr = *name;
        ptr++;
        name++;
    }
    //after I read the string, I set the next one to null
    ptr++;
    ptr= '\0';

    return 0;
}





/*
 * @brief  Append an additional component to the end of the pathname in path_buf.
 * @details  This function assumes that path_buf has been initialized to a valid
 * string.  It appends to the existing string the path separator character '/',
 * followed by the string given as argument, including its terminating null byte.
 * The length of the new string, including the terminating null byte, must be
 * no more than the size of path_buf.  The variable path_length is updated to
 * remain consistent with the length of the string in path_buf.
 *
 * @param  The string to be appended to the path in path_buf.  The string must
 * not contain any occurrences of the path separator character '/'.
 * @return 0 in case of success, -1 otherwise.
 */

int path_push(char *name) {
    int truth =0;
    int count =0;
    int it = 1;
    char* temp;
    temp = path_buf;
    //I check if the name was passed correctly
    if(name==NULL){
        return -1;
    }
    //I check if path_buf has a value in it
    //if it does not, there is an error
    if(*temp=='\0'){
        return -1;
    } else {
        while(truth==0){
            if(*temp!='\0'){
                //I iterate through path_buf until it reaches the end
                temp++;
            } else {
                //I have reached the end of the array
                if (count ==0){
                    //I set the first value after the original path to be '/'
                    *temp = '/';
                    temp++;

                    count++;
                } else {
                    //I begin to iterate through the name assigning it to path_buf
                    //if the name turns null, i exit
                    if(*name =='\0'){
                        //end of name string --> Action Completed
                        //I set the next character equal to a null character
                        temp++;
                        *temp = '\0';
                        return 0;
                        truth = -1;
                    } else {
                        //I set the next index of path_buf to the next index in name
                        *temp = *name;
                        temp++;
                        name++;
                    }

                }
            }
        }
    }


    return -1;
}

/*
 * @brief  Remove the last component from the end of the pathname.
 * @details  This function assumes that path_buf contains a non-empty string.
 * It removes the suffix of this string that starts at the last occurrence
 * of the path separator character '/'.  If there is no such occurrence,
 * then the entire string is removed, leaving an empty string in path_buf.
 * The variable path_length is updated to remain consistent with the length
 * of the string in path_buf.  The function fails if path_buf is originally
 * empty, so that there is no path component to be removed.
 *
 * @return 0 in case of success, -1 otherwise.
 */

int path_pop() {
    int count =0;
    int count2 =0;
    char* temp;
    temp = path_buf;
    int first =0;
    int truth =0;
    //I throw an error if path_buf is not initialized
    if(*temp=='\0'){
        return -1;
    }
    temp = path_buf;
    //I create a count that tells me when I've seen /
    while(*temp!='\0'){

        if(*temp=='/'){
            count++;
        }
        temp++;
    }
    //if there is no '/', I remove the entire string
    if(count ==0){
        *path_buf = '\0';
        //MAKE SURE THAT THIS IS CORRECT
        return 0;
    } else {
        temp = path_buf;
        while(truth==0){
            if(count2==count){
                //if we have reached the last '/'
                int it = 0;
                while(*(temp+it)!='\0'){
                    *(temp+it) = '\0';
                    it++;
                }
                truth = 1;
                return 0;
            } else if(*temp =='/'){
                count2++;
                if(count2==count){
                    //if we have reached the final '/'
                    //must also remove the final '/'
                    *temp = '\0';
                }
            }
            temp++;
            //temp++;
        }
    }

    return 0;
}


/*
 * @brief Deserialize directory contents into an existing directory.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory.  It reads (from the standard input) a sequence of DIRECTORY_ENTRY
 * records bracketed by a START_OF_DIRECTORY and END_OF_DIRECTORY record at the
 * same depth and it recreates the entries, leaving the deserialized files and
 * directories within the directory named by path_buf.
 *
 * @param depth  The value of the depth field that is expected to be found in
 * each of the records processed.
 * @return 0 in case of success, -1 in case of an error.  A variety of errors
 * can occur, including depth fields in the records read that do not match the
 * expected value, the records to be processed to not being with START_OF_DIRECTORY
 * or end with END_OF_DIRECTORY, or an I/O error occurs either while reading
 * the records from the standard input or in creating deserialized files and
 * directories.
 */
int deserialize_directory(int depth) {
    long size;
    int deDepth;
    int truth = 0;
    tickEntry=0;
    int first =0;
    int countOfEntry =0;
    while(truth ==0){
        int a = getchar();
        if(a==EOF){
            return -1;
            break;
        }
        int b = getchar();
        int c = getchar();
        int d = getchar(); //type
        countOfEntry = countOfEntry+4;
        //I check that the magic bytes are where they're supposed to be

        switch(d){
            case 2:
                if(tickEntry!=0){
                return -1;
                 }

                deDepth = getDeserializedDepth();
                countOfEntry = countOfEntry+4;
                if(deDepth!=(depth+1)){
                    return -1;

                }
                size = getDeserializedSize();
                countOfEntry = countOfEntry + 8;
                if(size!=16){
                    return -1;
                }
                //have to build directory
                if(first!=0){
                    char* partialName;
                    partialName = path_buf;
                    struct stat st = {0};
                    if(stat(partialName,&st)!=-1){
                        //somehow the directory alreayd exists
                        return -1;
                    }
                    mkdir(partialName, 0700);
                }
                first =1;
                depth++;
                tickEntry=2;
                break;
            case 3:
                deDepth = getDeserializedDepth();
                if(deDepth!=depth){
                    return -1;
                }
                //deserialize size
                long size = getDeserializedSize();
                if(size!=16){
                    return -1;
                }

                path_pop();

                depth--;
                tickEntry =2;
                break;
            case 4:
                //deserialize depth
                deDepth = getDeserializedDepth();
                if(deDepth!=depth){
                return -1;
                }
                //deserialize size
                size = getDeserializedSize();
                size = size -12 -16; //length of a name
                int mode = getDeserializedDepth(); //DOUBLE CHECK THIS

                //I check the mode
                if(S_ISDIR(mode)!=0){ //not sure if I need to use a pointer
                    tickEntry = 0;
                    //I give some sort of tick --> to be returned to other function
                } else if (S_ISREG(mode)!=0){
                    tickEntry = 1;
                    //I give a different tick
                }


                //the next 8 bytes are the size
                sizeDirEntry = getDeserializedSize();

                int val;
                char tempname;
                char* name;
                name = name_buf;
                for(int i=0; i<size;i++){
                    val = getchar();
                    tempname = val;
                    *name = tempname;
                    name++;
                }

                name = name_buf;
                path_push(name);
                //I clear the previous name
                name = name_buf;
                for(int i =0;i<size;i++){
                    *name = '\0';
                    name++;
                }
                break;
            case 5:
            if (deserialize_file(depth)!=0){
                return -1; }
                tickEntry =2;
                break;
            case 1:
                if((tickEntry!=2)|(depth>1)){
                    return -1;
                } else {
                    return 0;
                }
            default:
                    return -1;
        }
    }
    return 0;
}


/*
 * @brief Deserialize the contents of a single file.
 * @details  This function assumes that path_buf contains the name of a file
 * to be deserialized.  The file must not already exist, unless the ``clobber''
 * bit is set in the global_options variable.  It reads (from the standard input)
 * a single FILE_DATA record containing the file content and it recreates the file
 * from the content.
 *
 * @param depth  The value of the depth field that is expected to be found in
 * the FILE_DATA record.
 * @return 0 in case of success, -1 in case of an error.  A variety of errors
 * can occur, including a depth field in the FILE_DATA record that does not match
 * the expected value, the record read is not a FILE_DATA record, the file to
 * be created already exists, or an I/O error occurs either while reading
 * the FILE_DATA record from the standard input or while re-creating the
 * deserialized file.
 */
int deserialize_file(int depth){

    if(tickEntry!=1){
        //did not agree with previous DE
        return -1;
    }

    int deDepth = getDeserializedDepth();
    if(deDepth!=depth){
        return -1;
    }
    long size = getDeserializedSize();
    size = size-16;

    struct stat buf;
    char* name;
    name = path_buf; //NEED TO DOUBLE CHECK IF
    int truth = stat(name, &buf);
    if(truth ==0){
        //file already exists
        //need to check if clobber was given --> if it wasn't--> then I throw an error
        if(global_options!= 0x0c){
            //clobber has not been given
            return -1;
        }
    }
    //will open and clear the file if it exists, will create if not
    //allows me to write to file
    FILE *fptr = fopen(name, "w");
    if(fptr==NULL){
    }
    for(int i=0;i<size;i++){
        char byte = getchar();
        //I write the characters to the file
        fputc(byte,fptr);
    }
    path_pop();
    fclose(fptr);

    return 0;

}

/*
 * @brief  Serialize the contents of a directory as a sequence of records written
 * to the standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory to be serialized.  It serializes the contents of that directory as a
 * sequence of records that begins with a START_OF_DIRECTORY record, ends with an
 * END_OF_DIRECTORY record, and with the intervening records all of type DIRECTORY_ENTRY.
 *
 * @param depth  The value of the depth field that is expected to occur in the
 * START_OF_DIRECTORY, DIRECTORY_ENTRY, and END_OF_DIRECTORY records processed.
 * Note that this depth pertains only to the "top-level" records in the sequence:
 * DIRECTORY_ENTRY records may be recursively followed by similar sequence of
 * records describing sub-directories at a greater depth.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open files, failure to traverse directories, and I/O errors
 * that occur while reading file content and writing to standard output.
 */
int serialize_directory(int depth) {

    char* tempName = path_buf;

    filesRecursively(tempName, depth);

    return 0;
}

/*
 * @brief  Serialize the contents of a file as a single record written to the
 * standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * file to be serialized.  It serializes the contents of that file as a single
 * FILE_DATA record emitted to the standard output.
 *
 * @param depth  The value to be used in the depth field of the FILE_DATA record.
 * @param size  The number of bytes of data in the file to be serialized.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open the file, too many or not enough data bytes read
 * from the file, and I/O errors reading the file data or writing to standard output.
 */
int serialize_file(int depth, off_t size) {
    putchar(0x0C);
    putchar(0x0D);
    putchar(0xED);
    putchar(0x5);
    serializeDepth(depth);

    FILE *fptr;
    fptr = fopen(path_buf, "r");
    int charTemp;

    serializeSize(size+16);
    fclose(fptr);
    fptr = fopen(path_buf, "r");
    while((charTemp=getc(fptr)) != EOF){
        putchar(charTemp);
    }
    fclose(fptr);
    //I close the file
    return -1;
}

/**
 * @brief Serializes a tree of files and directories, writes
 * serialized data to standard output.
 * @details This function assumes path_buf has been initialized with the pathname
 * of a directory whose contents are to be serialized.  It traverses the tree of
 * files and directories contained in this directory (not including the directory
 * itself) and it emits on the standard output a sequence of bytes from which the
 * tree can be reconstructed.  Options that modify the behavior are obtained from
 * the global_options variable.
 *
 * @return 0 if serialization completes without error, -1 if an error occurs.
 */
int serialize() {
    int depth =0;
    int size =16;

    //start of transmission characters
    putchar(0x0C);
    putchar(0x0D);
    putchar(0xED);
    putchar(0x0);


    serializeDepth(depth);
    serializeSize(size);
    serializeStartDirectory(1);

    serialize_directory(0);


    serializeEndDirectory(1);


    //end of tansmission characters
    putchar(0x0C);
    putchar(0x0D);
    putchar(0xED);
    putchar(0x1);
    serializeDepth(depth);
    serializeSize(size);
    return 0;
}

/**
 * @brief Reads serialized data from the standard input and reconstructs from it
 * a tree of files and directories.
 * @details  This function assumes path_buf has been initialized with the pathname
 * of a directory into which a tree of files and directories is to be placed.
 * If the directory does not already exist, it is created.  The function then reads
 * from from the standard input a sequence of bytes that represent a serialized tree
 * of files and directories in the format written by serialize() and it reconstructs
 * the tree within the specified directory.  Options that modify the behavior are
 * obtained from the global_options variable.
 *
 * @return 0 if deserialization completes without error, -1 if an error occurs.
 */
int deserialize() {
    //if path buf does not exist --> I create it as a directory

    //I create a double pointer to hold the values seperated by the magic bytes
    if(deserializeStartTransmission(0)!=0){

        return -1;
    }
    if(deserialize_directory(0)!=0){
        return -1;
    }
     if(deserializeEndTransmission(0)!=0){
        return -1;
     }
     return 0;
 }



int deserializeDepth(int temp1, int temp2, int temp3, int temp4){
    int depth =0;
    int b1  = power(256,3)*temp1;
    int b2  = power(256,2)*temp2;
    int b3  = power(256,1)*temp3;
    int b4  = power(256,0)*temp4;

    depth = b1+b2+b3+b4;
    return depth;
}

long deserializeSize(int temp1, int temp2, int temp3, int temp4, int temp5, int temp6, int temp7, int temp8){
    long size =0;
    //assuming that the values given were in the order that they were read
    int b1  = power(256,7)*temp1;
    int b2  = power(256,6)*temp2;
    int b3  = power(256,5)*temp3;
    int b4  = power(256,4)*temp4;
    int b5  = power(256,3)*temp5;
    int b6  = power(256,2)*temp6;
    int b7  = power(256,1)*temp7;
    int b8  = temp8;

    size = b1+ b2+b3+b4+b5+b6+b7+b8;
    return size;
}


/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and -1 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and -1 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int validargs(int argc, char **argv)
{
    if(argc ==1){
        return -1;
    }



    char* tempPointer = *argv;
    /*
    if(validPath(tempPointer)!=0){
        return -1;
    }
    */
    path_init(tempPointer);
    argv++;
    //This is where I must return -1 if invalid argument
    //DEREFENCE ARGV TWICE TO GET FIRST INDEX
    char *cThing = "-c";
    char *sThing = "-s";
    char *dThing = "-d";
    char *pThing = "-p";
    char *hThing = "-h";

    int cVal = -1;
    int sVal = -1;
    int dVal = -1;
    int pVal = -1;


    int i=0;
    int truth = 0;

    for(i=0; i<argc-1; i++){
        //I go through all indexes of the array
        if((stringCompare(*argv, hThing)) ==0){ //-h

            //I make sure that -h was the first argument
            global_options= 0x1;
            if((sVal==0)||(pVal==0)||(dVal==0)||(pVal==0)||(cVal==0)){
                global_options=0x0;
                return -1;
            }
            return 0;

        } else if ((stringCompare(*argv, sThing))==0){// -s
            global_options= 0x2;
            //I check if sVal has been given already
            if(sVal == -1){
                sVal = 0;
            } else {
                global_options=0x0;
                return -1;
            }
            //I check if any lower commands were already given >> or deserialization
            //if they were, I throw an error
            if((dVal==0)||(pVal==0)||(cVal==0)){
                global_options=0x0;
                return -1;
            }
        } else if ((stringCompare(*argv, dThing))==0){//-d
            global_options = 0100;
            if(dVal == -1){
                dVal = 0;
            } else {
                global_options=0x0;
                return -1;
            }
            //I check if any lower commands were already given >> or, serialization
            //if they were, I throw an error

            if((sVal==0)||(pVal==0)||(cVal==0)){
                global_options=0x0;
                return -1;
            }
        } else if ((stringCompare(*argv, pThing))==0){//-p
            //global_options = 0xC;
            if(pVal == -1){
                pVal = 0;
            } else {
                global_options=0x0;
                return -1;
            }
            argv++;  //I go to the next index in the array

            //I check if it is a valid location

            if(validPath(*argv)!=0){
                //if it is not a valid path, I return an error
                global_options=0x0;
                return -1;
            }

            //I change the path
            if(path_init(*argv)!=0){
                return -1;
            }
            i++;
        } else if((stringCompare(*argv, cThing))==0){//-c
            global_options = 0xC;
            if(cVal == -1){
                cVal = 0;
            } else {
                global_options=0x0;
                return -1;
            }
            if(sVal==0){
                global_options=0x0;
                return -1;
            }
            if(dVal!=0){
                global_options=0x0;
                return -1;
            }

        } else{
            global_options=0x0;
            //After I go through all acceptable answers, if the truth does not change,
            //I return -1
            return -1;
        }

        argv++; // I go to the next index
    }

    return 0;
}