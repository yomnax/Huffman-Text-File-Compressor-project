//Project by: Ruba Issam - Basmalla Ehab - Saja Sheriff - Yomna Mohamed - Ledia Joseph

//NOTE: Turn off SDL-checks before running the code

#include<stdio.h>
#include<stdlib.h>
#include <string.h>


struct node {
    char data;
    int freq;
    char* str;
    struct node* ln = NULL;
    struct node* rn = NULL;
};
struct qnode {
    struct node* tree;
    struct qnode* next;
};

struct code {
    char c;
    code* next;
    char* ascii;
};

class files
{
public:
    qnode* header = nullptr;
    qnode* front = nullptr;
    qnode* rear = nullptr;
    qnode* temp = nullptr;
    code* h = nullptr;

    qnode* creatfnode(char data)
    {
        if (header == NULL)
        {
            qnode* newnode = (qnode*)malloc(sizeof(qnode));
            if (newnode == NULL)
            {
                free(newnode); // Free qnode memory if node allocation fails
                newnode = NULL;
                return NULL;
            }
            newnode->tree = (node*)malloc(sizeof(node)); // Allocate memory for the node
            if (newnode->tree == NULL)
            {
                free(newnode->tree); // Free qnode memory if node allocation fails
                newnode->tree = NULL;
                return NULL;
            }
            newnode->tree->data = data;
            newnode->tree->freq = 0;
            newnode->tree->str = (char*)malloc(256 * sizeof(char));
            newnode->tree->ln = NULL;  // Initialize left child pointer
            newnode->tree->rn = NULL;
            newnode->next = NULL;

        }
        else
        {
            qnode* temp = header;
            while (temp->next != NULL)
                temp = temp->next;

            qnode* newnode = (qnode*)malloc(sizeof(qnode));
            temp->next = newnode;
            if (newnode == NULL)
            {
                free(newnode); // Free qnode memory if node allocation fails
                newnode = NULL;
                return NULL;
            }
            newnode->tree = (node*)malloc(sizeof(node)); // Allocate memory for the node
            if (newnode->tree == NULL)
            {
                free(newnode->tree); // Free qnode memory if node allocation fails
                newnode->tree = NULL;
                return NULL;
            }
            newnode->tree->data = data;
            newnode->tree->freq = 0;
            newnode->tree->str = (char*)malloc(256 * sizeof(char));
            newnode->tree->ln = NULL;  // Initialize left child pointer
            newnode->tree->rn = NULL;
            newnode->next = NULL;

        }
    }

    void readfile(char filename[])
    {
        FILE* file;
        errno_t err;
        char ch;
        // Open the file
        err = fopen_s(&file, filename, "r");
        // Check if the file exists
        if (file == NULL) {
            printf("File '%s' not found.\n", filename);
            return;
        }

        qnode* ptr = NULL;
        // Read characters from the file until EOF
        while ((ch = fgetc(file)) != EOF)
        {
            ptr = header;
            if (ptr == NULL)
            {
                header = creatfnode(ch);
                ptr = header;
                ptr->tree->freq++;
            }
            else
            {
                while (ptr->next != NULL && ptr->tree->data != ch)
                {
                    ptr = ptr->next;
                }

                if (ptr->tree->data == ch)
                    ptr->tree->freq++;

                else
                {
                    qnode* newnode = creatfnode(ch);
                    if (newnode != NULL)
                    {
                        newnode->tree->freq = 1;
                        ptr->next = newnode;
                        ptr = newnode;
                    }
                    else
                    {
                        printf("Memory allocation failed.\n");
                        fclose(file);
                        return;
                    }
                }
            }
        }
        fclose(file);
    }

   

    // Sorting nodes based on frequency
    void  sorting()
    {
        qnode* p1 = header;
        while (p1 != NULL)
        {
            qnode* p2 = p1->next;
            while (p2 != NULL) {
                if (p2->tree->freq < p1->tree->freq)
                {
                    char temp_data = p1->tree->data;
                    int temp_freq = p1->tree->freq;
                    p1->tree->data = p2->tree->data;
                    p1->tree->freq = p2->tree->freq;
                    p2->tree->data = temp_data;
                    p2->tree->freq = temp_freq;
                }
                p2 = p2->next;
            }
            p1 = p1->next;
        }

        rear = header;
        front = header;
        while (rear->next != NULL)
        {
            rear = rear->next;
        }

    }

    void printfile()
    {
        qnode* print = header;
        while (print != NULL)
        {
            printf("%c : %d \n", print->tree->data, print->tree->freq);
            print = print->next;
        }

    }

    struct node* dequeue() {
        if (front == NULL) {
            return NULL;
        }

        qnode* temp = front;
        front = front->next;

        if (front == NULL) {
            rear = NULL;
        }

        return temp->tree;
    }

    //Creating the huffman tree
    node* createtree() {
        while (front->next != nullptr)
        {
            node* ptr1 = dequeue();
            node* ptr2 = dequeue();
            qnode* newnode = (qnode*)malloc(sizeof(qnode));
            if (newnode == NULL) {
                free(newnode);
                return NULL;
            }
            newnode->tree = (node*)malloc(sizeof(node));
            if (newnode->tree == NULL)
            {
                free(newnode->next);
                return NULL;
            }
            newnode->tree->data = '*';

            newnode->tree->freq = ptr1->freq + ptr2->freq;
            if (ptr1->freq > ptr2->freq)
            {
                newnode->tree->rn = ptr1;
                newnode->tree->ln = ptr2;
            }
            else if (ptr1->freq < ptr2->freq)
            {
                newnode->tree->ln = ptr1;
                newnode->tree->rn = ptr2;
            }
            else
            {
                newnode->tree->rn = ptr1;
                newnode->tree->ln = ptr2;
            }
            enqueue(newnode);

        }
        return front->tree;
    }

    //put the new tree node in the tree
    void enqueue(qnode* entry)
    {
        qnode* temp = NULL;
        if (front == nullptr || entry->tree->freq < front->tree->freq)
        {
            entry->next = front;
            front = entry;
        }
        else
        {
            temp = front;
            while (temp->next != NULL && temp->next->tree->freq <= entry->tree->freq)
            {
                temp = temp->next;
            }
            entry->next = temp->next;
            temp->next = entry;

        }

    }
   

    //Creating linkedlist for the character and its binary code
    code* createCnode(char str[], char data, code** head) {

        if (*head == NULL)
        {
            code* newC = (code*)malloc(sizeof(code));
            if (newC == NULL)
                return NULL;
            newC->c = data;
            newC->ascii = strdup(str);
            newC->next = NULL;
            *head = newC;
        }
        else
        {
            code* it = *head;

            while (it->next != NULL)
                it = it->next;

            it->next = (code*)malloc(sizeof(code));
            if (it->next == NULL)
                return NULL;
            it->next->c = data;
            it->next->ascii = strdup(str);
            it->next->next = NULL;
        }
        return *head;
    }

    //traverse the huffman tree to get the binary length code (left =0 , right= 1)
    void traverse(node* root, char* s) {
        if (root == NULL)
            return;
        char leftstr[256], rightstr[256]; //2^8 = 256, as the max bit is 8
        strcpy(leftstr, s);
        strcpy(rightstr, s);

        if (root->data != '*') {
            createCnode(s, root->data, &h); // Create a code node for leaf nodes
        }

        traverse(root->ln, strcat(leftstr, "0"));

        traverse(root->rn, strcat(rightstr, "1"));

    }

    //to call the function in the main
    void huffmancode(node* root) {
        char code[256] = "";
        traverse(root, code);
    }

    //printing the charcater and its binary code 
    void printCodes(code* head) {
        printf("Character\tBinary Code\n");
        while (head != NULL) {
            printf("%c\t\t%s\n", head->c, head->ascii);
            head = head->next;
        }
    }

    //putting the binary code on the .cod file 
    void codefile() {
        FILE* files;
        files = fopen("files.cod", "w");

        code* t = h;
        while (t != NULL)
        {
            fputc('(', files);
            fputc(t->c, files);
            fputc(')', files);
            fprintf(files, " ");
            fprintf(files, t->ascii);
            fprintf(files, "\n");
            t = t->next;
        }
        fclose(files);

    }

    //compressing the file and adding to .com file
    void encode(char filename[]) {
        FILE* file;
        errno_t err;
        char ch;
        err = fopen_s(&file, filename, "r");
        if (file == NULL) {
            printf("File '%s' not found.\n", filename);
            return;
        }

        FILE* hash;
        hash = fopen("Encoding.com", "wb");
        if (hash == NULL) {
            printf("Failed to open the output file 'Encoding.com'.\n");
            fclose(file);
            return;
        }

        code* temp;
        int count = 0;
        unsigned char buffer = 0;

        //put padding information in the first byte
        unsigned char padding = 0;
        fwrite(&padding, sizeof(padding), 1, hash);

        // Read characters from the input file
        while ((ch = fgetc(file)) != EOF)
        {
            temp = h;
            while (temp != NULL) {
                if (ch == temp->c) {
                    //Write the Huffman code in the buffer
                    for (int i = 0; i < strlen(temp->ascii); i++) {
                        if (temp->ascii[i] == '1') {
                            buffer = (buffer << 1) | 1;
                        }
                        else {
                            buffer = (buffer << 1) | 0;
                        }
                        count++;

                        //put the buffer in the file if it is 8 bits
                        if (count == 8) {
                            fputc(buffer, hash);
                            count = 0;
                            buffer = 0;
                        }
                    }
                    break;
                }
                temp = temp->next;
            }
        }

        //padding
        if (count > 0) {
            padding = 8 - count;
            buffer = buffer << padding;
            fputc(buffer, hash);
        }
        else
            padding = 0;

        // Go back to the beginning and write the padding information
        fseek(hash, 0, SEEK_SET);
        fwrite(&padding, sizeof(padding), 1, hash);

        fclose(file);
        fclose(hash);

    }

    //Decompression 

    void decomp() {
        FILE* cod;
        cod = fopen("files.cod", "r");

        if (cod == NULL) {
            printf("Failed to open the code file.\n");
            return;
        }

        // Read the Huffman codes from the .cod file
        char c;
        char ascii[256];
        code* h1 = NULL;
        while (fscanf(cod, " (%c) %s", &c, ascii) != EOF) {
            h1 = createCnode(ascii, c, &h1);
        }
        fclose(cod);

        FILE* com;
        com = fopen("Encoding.com", "rb");

        if (com == NULL) {
            printf("Failed to open the encoded file 'Encoding.com'.\n");
            return;
        }

        FILE* decom;
        decom = fopen("decompressed.txt", "w");
        if (decom == NULL)
        {
            printf("Failed to create the output file 'decompressed.txt'.\n");
            fclose(decom);
            return;
        }

        unsigned char buffer;
        int count = 0;
        char bit[257] = "";
            code * tmp = h1;

        unsigned char padding = 0;
        fread(&padding, sizeof(padding), 1, com); //read the padding info


        fseek(com, 0, SEEK_END); // this func get the end of the file
        long file_size = ftell(com);
        fseek(com, sizeof(padding), SEEK_SET);

        while (fread(&buffer, sizeof(buffer), 1, com))
        {
            long current_position = ftell(com); //get the position of the file now
            if (current_position != file_size) { //not in the last byte of the file
                for (int i = 7; i >= 0; i--) {
                    if (buffer & (1 << i)) {
                        bit[count++] = '1';
                    }
                    else {
                        bit[count++] = '0';
                    }
                    bit[count] = '\0';

                    tmp = h1;
                    while (tmp != NULL) {
                        if (strcmp(bit, tmp->ascii) == 0) {
                            fputc(tmp->c, decom);
                            // for (int j = 0; j < count; j++) {
                            //    bit[j] = '\0';
                             //}
                            bit[0] = '\0';
                            count = 0;

                            break;
                        }
                        tmp = tmp->next;
                    }
                }
            }
            else if (current_position == file_size) { //in the last byte of the file
                for (int i = 7; i >= padding; i--) {
                    if (buffer & (1 << i)) {

                        bit[count++] = '1';
                    }
                    else {
                        bit[count++] = '0';
                    }
                    bit[count] = '\0';

                    tmp = h1;
                    while (tmp != NULL) {
                        if (strcmp(bit, tmp->ascii) == 0) {
                            fputc(tmp->c, decom);
                            //for (int j = 0; j < count; j++) {
                            //    bit[j] = '\0';
                            //}
                            bit[0] = '\0';
                            count = 0;

                            break;
                        }
                        tmp = tmp->next;
                    }

                }
            }
        }

        fclose(decom);
        fclose(com);

    }



};

/* //To print what's in the .cod file (delete later)
void printBinary(unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (byte >> i) & 1);
    }
}*/

int main()
{
    char str[20];
    printf("Enter your file name that you want to compress (Example: FileName.txt): ");
    scanf("%s", str);
    files f;

    f.readfile(str);
    f.sorting();
    f.printfile();
    node* root = f.createtree();
    f.huffmancode(root);
    f.printCodes(f.h);
    f.codefile();
    f.encode(str);

    FILE* fl = fopen("Encoding.com", "rb");
    if (fl == NULL) {
        printf("Failed to open the file 'Encoding.com'.\n");
        return 1;
    }

    /* //To print what's in the .cod file (delete later)
    int byte;
    while ((byte = fgetc(fl)) != EOF) {
        printBinary((unsigned char)byte);
        printf(" ");
    }
    fclose(fl);*/

    f.decomp();
    return 0;
}
