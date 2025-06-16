#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdio_ext.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEFAULT_FILE				"address_book.csv"

/* Length of a contact name  */
#define NAME_LEN					32
/* Length of a phone number */
#define NUMBER_LEN					32
/* Length of an email address */
#define EMAIL_ID_LEN				32



typedef enum
{
	e_fail = -1,
	e_back,
	e_success,
	e_no_match,
	e_new_line,
	e_free
} Status;

typedef enum 
{
	e_exit,
	e_add_contact,
	e_search_contact,
	e_edit_contact,
	e_delete_contact,
	e_list_contacts,
	e_save
}operation;

typedef struct
{
	char name[NAME_LEN];
	char phone_numbers[5][NUMBER_LEN];
	char email_addresses[5][EMAIL_ID_LEN];
	int si_no, phone_no_count, email_count;
} ContactInfo;

typedef struct
{	
    FILE *fp;
	ContactInfo *list;
	int count;	
} AddressBook;

typedef enum
{
	e_name,
	e_phone_number,
	e_emailid
}type;


void menu_header(const char *str);
void main_menu(void);
Status menu(AddressBook *address_book);
Status save_prompt(AddressBook *address_book);
Status add_contacts(AddressBook *address_book);
Status search_contact(AddressBook *address_book, operation opr);
Status edit_contact(AddressBook *address_book);
Status delete_contact(AddressBook *address_book);
Status list_contacts(AddressBook *address_book);
void sub_menu(AddressBook *address_Book, int index);
Status search(AddressBook *address_book, type);
Status contact_adding(AddressBook *address_book, int index, char *);
Status exiting(operation type);
Status load_contacts(AddressBook *address_book);
Status validate_phone_number(char *new, AddressBook *address_book);
Status validate_email_id(char *new, AddressBook *address_book);
#endif
