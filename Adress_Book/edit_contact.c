#include "main.h"
Status edit_contact(AddressBook *address_book)
{
    int res, index;
    if((res = search_contact(address_book, e_edit_contact)) == e_fail)
        return e_fail;
    else if(res == e_no_match)
        return e_no_match;
    else if(res == e_success)
    {
        printf("Select a Serial Number(S.No) to edit : ");
        scanf("%d",&index);
        contact_adding(address_book, index, "Search Contact to edit by :\n");
        return e_success;
    }

}