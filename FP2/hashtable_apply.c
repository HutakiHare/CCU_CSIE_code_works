unsigned int hash(char *name){ //forming correspondence(inpur: name, output: hash value)
    int length = strnlen(name, NAME_SIZE);
    unsigned int hash_value = 0;
    for(int i = 0; i < length; i++){
        hash_value += name[i];
        hash_value = (hash_value * name[i]) % TABLE_SIZE;
    }
    return hash_value;
}

void init_hash_table(){
    for(int i = 0; i < TABLE_SIZE; i++){
        hash_table[i] = NULL;
    }
}

void print_table(){
    printf("Start\n");
    for(int i = 0; i < TABLE_SIZE; i++){
        if(hash_table[i] == NULL){
            printf("\t%d\t---\n", i);
        }else if(hash_table[i] == DELETED_NODE){
            printf("\t%d\t---<deleted>\n", i);
        }else{
            printf("\t%d\t%s\n", i, hash_table[i] -> name);
        }
    }
    printf("End\n");
}

bool hash_table_insert(struct event_content *p){ //
    if(p == NULL){
        return false;
    }
    int index = hash(p -> name); // hash using name in p
    for(int i = 0; i < TABLE_SIZE; i++){
        int try = (i + index) % TABLE_SIZE; //if collision ocurr, try to use linear search to find a space to fit in
        if(hash_table[try] == NULL || hash_table[try] == DELETED_NODE){
            hash_table[try] = p; //if finding a space
            return true; //did find a space
        }
    }
    return false; //did not find a space
}

Event_content *hash_table_lookup(char *name){
    int index = hash(name); // hash using name directly
    //printf("index: %d\n", index);
    for(int i = 0; i < TABLE_SIZE; i++){
        int try = (i + index) % TABLE_SIZE; //if collision ocurr, try to use linear search to find if it found a space to fit in
        if(hash_table[try] == NULL){
            return false;
        }
        if(hash_table[try] == DELETED_NODE){
            continue;
        }
        if(strncmp(hash_table[try] -> name, name, TABLE_SIZE) == 0){
            return hash_table[try]; //if found it, return the node
        }
    }
    return NULL;
}

Event_content *hash_table_delete(char *name){
    int index = hash(name); // hash using name directly
    //printf("index: %d\n", index);
    for(int i = 0; i < TABLE_SIZE; i++){
        int try = (i + index) % TABLE_SIZE; //if collision ocurr, try to use linear search to find if it found a space to fit in
        if(hash_table[try] == NULL){
            return NULL;
        }
        if(hash_table[try] == DELETED_NODE){
            continue;
        }
        if(strncmp(hash_table[try] -> name, name, TABLE_SIZE) == 0){
            struct event_content *tmp = hash_table[try];
            hash_table[try] = DELETED_NODE;
            return tmp; //if found it, return the node
        }
    }
    return NULL;
}