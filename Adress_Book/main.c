
#include "main.h"
/*
#include "address_book_fops.h"
#include "address_book_log.h"
#include "address_book_menu.h"
#include "address_book.h"
*/
int contact_num;
void main_menu(void)
{
    menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

void menu_header(const char *str)
{
	fflush(stdout);

	system("clear");

	printf("#######  Address Book  #######\n");
	if (str[0] != '\0')
	{
		printf("#######  %s\n", str);
	}
}

int main()
{
    static int wait=0;
    int option = 0, res;
    AddressBook address_book;
    address_book.list = NULL;
    address_book.count = 0;
    
    do
    {
        main_menu();
        scanf("%d", &option);
            
        switch (option)
        {
        case 0:
            break;  //EXIT
        case 1:
            if(add_contacts(&address_book) == e_success)
            {
                printf("contact added successfully\n");
            }
            else
            {
                printf("contact adding failed\n");
            }
            break;
        case 2:
            // search contact
            if((res = search_contact(&address_book, e_search_contact)) == e_fail)
            {
                printf("FAIL : No conatacts present to search\n");
            }
            else if(res == e_no_match)
            {
                printf("No mathcing contact found!\n");
            }
            break;
        case 3:
            // edit contact
            
            if((res = edit_contact(&address_book)) == e_success)
                printf("SUCCESS : contact edited");
            else if(res == e_fail)
                printf("FAILURE : contact not edited");
            else if(res == e_no_match)
                printf("NO matching contact found !!!");
            break;
        case 4:
            // delete contact
            
            if(delete_contact(&address_book) == e_success)
                printf("Contact deleted Successfully\n");
            else
                printf("Contact deleting Failed\n");
            break;
        case 5:
            
            if((res = list_contacts(&address_book)) == e_success)
                printf("SUCCESS : list contacts");
            else if(res = e_free)
                printf("NO contacts present");
            else
                printf("FAILURE : list contacts");
            break;
        case 6:
            // save contact
            
            if(save_prompt(&address_book) == e_success)
                printf("SUCCESS : saved the contacts");
            else
                printf("FAIL : contacts saving");
            break;
        
        default:
            printf("Please enter valid input\n");
            break;
        }
        sleep(1);
        
    }while(option != 0);
    return 0;
}

