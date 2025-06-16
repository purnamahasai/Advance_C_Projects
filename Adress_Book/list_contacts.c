#include"main.h"
Status list_contacts(AddressBook *address_book)
{
    if(address_book->count == 0)
        return e_free;
    int ind=0;   
    char flag = 0, stop;
    for(int i=0; i<address_book->count; i++)
    {        
        /*if si no not -1 display it */
        if(address_book->list[i].si_no == -1)
            continue;
        
        if(flag == 0)
        {
            menu_header("Search Result :\n");
            flag++;
            printf("=============================================================================================\n");
            printf("%5s:%20s:%20s:%20s\n","S.No", "Name", "Phone No", "Email ID");
            printf("=============================================================================================\n");
        }
        printf("%5d:%20s:%20s:%20s\n",ind++, address_book->list[i].name, address_book->list[i].phone_numbers[0], address_book->list[i].email_addresses[0]);
        for(int j=1; j<5; j++)
        {
            printf("%5s:%20s:%20s:%20s\n", "\0", "\0", address_book->list[i].phone_numbers[j], address_book->list[i].email_addresses[j]);
        }
        
    }
    if(flag)
    {
        printf("=============================================================================================\n");
        printf("Press [q] | Cancel : ");
        scanf(" %c", &stop);
        if(stop == 'q')
            return e_success;
    }
    else
        return e_free;
    
}