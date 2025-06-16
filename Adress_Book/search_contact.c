#include"main.h"
Status search_contact(AddressBook *address_book, operation opr)
{
    if(address_book->count == 0)
        return e_fail;
    int option = 0, res, r;
    menu_header("Search contact by :\n");
    printf("0. Back \n""1. Name \n""2. Phone No \n""3. Email ID \n\n");
    printf("Please select an option : ");
    scanf("%d", &option);
    switch(option)
    {
        case 0:
            break;
        case 1:
            printf("Enter the Name : ");
            if((r=search(address_book, e_name)) == e_fail)
                return e_fail;
            else if(r==e_no_match)
                return e_no_match;
            if((res = exiting(opr)) == e_success)
                return e_success;
            else if(res == e_back)
                return e_back;
            return e_fail;
        
            break;
        case 2:
            printf("Enter the Phone number : ");
            if((r=search(address_book, e_phone_number))==e_fail)
                return e_fail;
            else if(r==e_no_match)
                return e_no_match;
            if((res = exiting(opr)) == e_success)
                return e_success;
            else if(res == e_back)
                return e_back;
            return e_fail;
            break;
        case 3:
            printf("Enter the Email ID : ");
            if((r=search(address_book, e_emailid))==e_fail)
                return e_fail;
            else if(r==e_no_match)
                return e_no_match;
            if((res = exiting(opr)) == e_success)
                return e_success;
            else if(res == e_back)
                return e_back;
            return e_fail;
            break;
        default:
        printf("Pass valid arguments for searching\n");
        break;
    }
}
Status search(AddressBook *address_book, type field)
{
    char searching_contact[32];
    scanf(" %[^\n]", searching_contact);
    char flag = 0, stop;
    switch (field)
    {
    case e_name:
        for(int i=0; i<address_book->count; i++)
        {
            if(address_book->list[i].si_no == -1)
            continue;
            if(!strcmp(searching_contact, address_book->list[i].name))
            {
                
                /*display it */
                
                if(flag == 0)
                {
                    menu_header("Search Result :\n");
                    flag++;
                    printf("=============================================================================================\n");
                    printf("%5s:%20s:%20s:%20s\n","S.No", "Name", "Phone No", "Email ID");
                    printf("=============================================================================================\n");
                }
                printf("%5d:%20s:%20s:%20s\n",address_book->list[i].si_no, address_book->list[i].name, address_book->list[i].phone_numbers[0], address_book->list[i].email_addresses[0]);
                for(int j=1; j<5; j++)
                {
                    printf("%5s:%20s:%20s:%20s\n", "\0", "\0", address_book->list[i].phone_numbers[j], address_book->list[i].email_addresses[j]);
                }
            }
            
        }
        if(flag)
        {
            printf("=============================================================================================\n");
            return e_success;
        }
        else
            return e_no_match;
        return e_fail;
        break;
    case e_phone_number:
        for(int i=0; i<address_book->count; i++)
        {
            if(address_book->list[i].si_no == -1)
            continue;
            for(int j=0; j<address_book->list[i].phone_no_count; j++)
                {
                    if(!strcmp(searching_contact, address_book->list[i].phone_numbers[j]))
                    {
                        
                        /*display it */
                        menu_header("Search Result :\n");
                        if(flag == 0)
                        {
                            flag++;
                            printf("=============================================================================================\n");
                            printf("%5s:%20s:%20s:%20s\n","S.No", "Name", "Phone No", "Email ID");
                            printf("=============================================================================================\n");
                        }
                        printf("%5d:%20s:%20s:%20s\n",address_book->list[i].si_no, address_book->list[i].name, address_book->list[i].phone_numbers[0], address_book->list[i].email_addresses[0]);
                        for(int j=1; j<5; j++)
                        {
                            printf("%5s:%20s:%20s:%20s\n", "\0", "\0", address_book->list[i].phone_numbers[j], address_book->list[i].email_addresses[j]);
                        }
                    }
                }
        }
        if(flag)
        {
            printf("=============================================================================================\n");
            return e_success;
        }
        else
            return e_no_match;
        return e_fail;       
        break;
    case e_emailid:
        for(int i=0; i<address_book->count; i++)
        {
            if(address_book->list[i].si_no == -1)
            continue;
            for(int j=0; j<address_book->list[i].phone_no_count; j++)
                {
                  
                    if(!strcmp(searching_contact, address_book->list[i].email_addresses[j]))
                    {
                        
                        /*display it */
                        menu_header("Search Result :\n");
                        if(flag == 0)
                        {
                            flag++;
                            printf("=======================================================================================================================\n");
                            printf(": %5s : %20s : %20s : %20s :\n","S.No", "Name", "Phone No", "Email ID");
                            printf("=======================================================================================================================\n");
                        }
                        printf(": %5d : %20s : %20s : %20s :\n",address_book->list[i].si_no, address_book->list[i].name, address_book->list[i].phone_numbers[0], address_book->list[i].email_addresses[0]);
                        for(int j=1; j<5; j++)
                        {
                            printf("%5s:%20s:%20s:%20s\n", "\0", "\0", address_book->list[i].phone_numbers[j], address_book->list[i].email_addresses[j]);
                        }
                    }
            }
        }

        if(flag)
        {
            printf("=======================================================================================================================\n");
            return e_success;
        }
        else
            return e_no_match;
        return e_fail;
        break;
    
    default:
        break;
    } 
    return e_fail;   
}