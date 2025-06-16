#include"main.h"
/* displaying contact info */
void sub_menu(AddressBook *address_book, int index)
{  
    printf("0. Back :\n");
    printf("1. Name : %s%c", address_book->list[index].name, '\n');

    printf("2. Phone No 1 : %s%c", address_book->list[index].phone_numbers[0], '\n');
    for(int i=1; i<address_book->list[index].phone_no_count; i++)
    {
        printf("   Phone No %d : %s%c",i+1, address_book->list[index].phone_numbers[i], '\n');
        
    }

    printf("3. Email ID : %s%c", address_book->list[index].email_addresses[0], '\n');
    for(int i=1; i<address_book->list[index].email_count; i++)
    {
        printf("   Email ID %d : %s%c",i+1, address_book->list[index].email_addresses[i], '\n');
        
    }
    printf("Please select an option : ");
}

Status exiting(operation type)
{
    char stop = 0;
    if(type == e_search_contact)
    {
    printf("Press [q] | Cancel : ");
        scanf(" %c", &stop);
        if(stop == 'q')
        return e_success;
        return e_fail;          
    }
    else if(type == e_edit_contact)
    {
        printf("Press : [q] = Cancel . [s] = Select : ");
        scanf(" %c", &stop);
        if(stop == 's')
            return e_success;
        else if(stop == 'q')
            return e_back;
        return e_fail; 
    }
}

/* adding contact*/
Status contact_adding(AddressBook *address_book, int index, char *str)
{
    
    
    int option = 0;
    do
    {
        menu_header(str);
        sub_menu(address_book, index);  //display contact info and prompt select option to user
        scanf("%d", &option);
        switch (option)
        {
            case 1:
                printf("Please enter name : ");
                scanf(" %[^\n]", address_book->list[index].name);   //reaing name
                break;
            case 2:
                    //reading and validating the phone number
                    if(address_book->list[index].phone_no_count < 5)
                    {
                        printf("Please enter  phone number %d: [Please re enter the same option for alternate phone number] ", address_book->list[index].phone_no_count+1);
                        scanf(" %[^\n]", address_book->list[index].phone_numbers[address_book->list[index].phone_no_count]);


                        //validating phone number with previously stored once
                        if(validate_phone_number(address_book->list[index].phone_numbers[address_book->list[index].phone_no_count], address_book)==e_fail)
                        {
                            //if phone number is not valid store simply discard it and make it NULL
                            strcpy(address_book->list[index].phone_numbers[address_book->list[index].phone_no_count] , "\0");
                            printf("Phone Number already present Plese enter new Phone Number\n");                   
                        }
                        else
                        {
                            //if phone number is valid store it and increment phoe number count
                            address_book->list[index].phone_no_count++;
                        }
                    }
                
                break;
            case 3:
                //reading and validating email id
                printf("Please enter  Email ID %d: [Please re enter the same option for alternate Email ID] ", address_book->list[index].email_count+1);
                    scanf(" %[^\n]", address_book->list[index].email_addresses[address_book->list[index].email_count]);
                    //validating email id
                    if(validate_email_id(address_book->list[index].email_addresses[address_book->list[index].email_count], address_book)==e_fail)
                    {
                        //if already present discard it and make it NULL
                        strcpy(address_book->list[index].email_addresses[address_book->list[index].email_count] , "\0");
                        printf("Email ID already present Plese enter new Email ID\n");                   
                    }
                    else
                    {
                        //if email id is valid store it
                        address_book->list[index].email_count++;
                    }
                break;
            case 0:
                break;  //0 to go back
            default:
                printf("Enter valid option \n");
                break;
        }
        sleep(1);

    } while (option);
    return e_success;
}
/* PHONE NUMBER VALIDATION  */
Status validate_phone_number(char *new, AddressBook *address_book)
{
    for(int i=0; i<=address_book->count; i++)
    {
        for(int j=0; j<address_book->list[i].phone_no_count; j++)
        {
            if(address_book->list[i].phone_numbers[j] == NULL)
                break;
            if(strcmp(new, address_book->list[i].phone_numbers[j]) == 0)
                return e_fail;
        }
    }
    return e_success;
}
/*  EMAIL ID VALIDATION */
Status validate_email_id(char *new, AddressBook *address_book)
{
    for(int i=0; i<=address_book->count; i++)
    {
        for(int j=0; j<address_book->list[i].email_count; j++)
        {
            if(address_book->list[i].email_addresses[j] == NULL)
                break;
            if(strcmp(new, address_book->list[i].email_addresses[j]) == 0)
                return e_fail;
        }
    }
    return e_success;
}