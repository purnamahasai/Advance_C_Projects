#include"main.h"
extern int contact_num;
Status delete_contact(AddressBook *address_book)
{
    int res, index;
    if((res = search_contact(address_book, e_edit_contact)) == e_fail)
        return e_fail;
    else if(res == e_success)
    {
        printf("Select a Serial Number(S.No) to Delete : ");
        scanf(" %d",&index);
        address_book->list[index].si_no = -1;
        contact_num--;
        return e_success;
    }
    
}