#include "main.h"
extern int contact_num;
Status save_prompt(AddressBook *address_book)
{
    if(contact_num == 0)
        return e_fail;
    address_book->fp = fopen(DEFAULT_FILE, "w");

	if (address_book->fp == NULL)
	{
		return e_fail;
	}
    fprintf(address_book->fp, "#%d#\n",contact_num);
    for(int i=0; i<address_book->count; i++)
    {        
        if(address_book->list[i].si_no == -1)
            continue;
        fprintf(address_book->fp, "%s,",address_book->list[i].name);
        fprintf(address_book->fp, "%s,",address_book->list[i].phone_numbers[0]);
        fprintf(address_book->fp, "%s,",address_book->list[i].email_addresses[0]);
        for(int j=1; j<5; j++)
        {
            if(fprintf(address_book->fp, "%s",address_book->list[i].phone_numbers[j]) != 0)
                fprintf(address_book->fp, ",");
            if(fprintf(address_book->fp, "%s",address_book->list[i].email_addresses[j]) != 0)
                fprintf(address_book->fp, ",");
        }
        fprintf(address_book->fp, "\n");

    }
	fclose(address_book->fp);
	return e_success;      
        
}