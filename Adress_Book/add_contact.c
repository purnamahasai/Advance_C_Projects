#include"main.h"
extern int contact_num;
Status add_contacts(AddressBook *address_book)
{
    address_book->list = realloc(address_book->list, ((address_book->count)+1) * sizeof(ContactInfo));    //memory allocation
    if(address_book->list== NULL) 
        return e_fail;
    //int option=0;

    address_book->list[address_book->count].si_no = address_book->count;

    // putting NULL to remove garbage for name, phone number, mailid
    strcpy(address_book->list[address_book->count].name , "\0");

    for(int i=0; i<5; i++)
    {
        strcpy(address_book->list[address_book->count].phone_numbers[i] , "\0");
        strcpy(address_book->list[address_book->count].email_addresses[i] , "\0");
    }    
    address_book->list[address_book->count].phone_no_count = 0;
    address_book->list[address_book->count].email_count = 0;

    //ADDING CONTACT
    contact_adding(address_book, address_book->count, "Add contact :\n");

    address_book->count += 1;   //increment count
    contact_num = address_book->count;

    return  e_success;  
}