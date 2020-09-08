#include "../header/main.h"

void DeleteFile(char *target_folder, char *target_file)
{
    char *pwd_path = getenv("PWD");

    //concat for pwd, inorder  to find target_file currently 
    size_t size_of_pwd = ( (strlen(pwd_path) + strlen(target_file) ) + 1);
    char *target_file_location = cat_path(size_of_pwd,
                                             pwd_path,
                                             target_file, 
                                             KEEP_HEAD);

    
    if ((access(target_file_location, F_OK)) == 0)
    {
        //concat for pwd, inorder to delete target_file later
        size_t size_of_del_loc = ( strlen(target_folder) + (strlen(target_file) * 2) + 1);
        char *delete_location_subfolder = cat_path(size_of_del_loc,
                                            target_folder,
                                            target_file,
                                            KEEP_HEAD);

        //concat for pwd, inorder to store trace file with target_file
        char *extension = "_trace.rtf\0";

        size_t size_of_trace_loc = ( strlen(delete_location_subfolder) + strlen(target_file) + strlen(extension) + 2);
        char *trace_file_location = cat_path(size_of_trace_loc,
                                                delete_location_subfolder,
                                                target_file,
                                                KEEP_HEAD);

            strcat(trace_file_location, extension);

        //a directory is needed to store the target_file and the TraceFile containing the original pwd
        int file_exists;
        if (!((access(delete_location_subfolder, F_OK)) == 0))
        {
            file_exists = mkdir(delete_location_subfolder, 0755);  

            if (file_exists == 0)
            {
                InitTraceFile(trace_file_location, target_file_location);
            }

            else 
            {
                printf("Error, cannot create - %s", delete_location_subfolder);
            }  
        }

        //full pwd to where target_file will go on deletion
        char *delete_location = cat_path(size_of_del_loc,
                                delete_location_subfolder,
                                target_file,
                                KEEP_HEAD);
    
        //aka: move target_file_location -> delete_location_file
        rename(target_file_location, delete_location);

        free(target_file_location);
        free(delete_location);
        free(delete_location_subfolder);
        free(trace_file_location);
    }

    else
    {
        fprintf(stderr, "%s - non existent", target_file_location);
    }
}

void ListDir(char *target_folder, int size_details) 
{ 
    struct stat file_stat;

    //readdir() will access directory_stream returning a pointer to a entry to be accessed
    struct dirent *directory_entry;  

    //opendir returns a stream of files in the directory
    DIR *directory_stream = opendir(target_folder);

    if (directory_stream)
    {
        //access the next dirent in the directory_stream, filtering out '.' ".." path directives
        while (((directory_entry = readdir(directory_stream)) != NULL) &&
                (!(cmpstr(directory_entry->d_name, "."))) &&
                (!(cmpstr(directory_entry->d_name, ".."))) )
        {      
            //because files in .trash are nested in a self named folder we need to append for that depth for the full pwd     
            size_t filename_size = (strlen(target_folder) + (strlen(directory_entry->d_name) * 2)) + 2;
            char *file_path = cat_path(filename_size,
                                       cat_path(filename_size - strlen(directory_entry->d_name), 
                                                target_folder, 
                                                directory_entry->d_name,
                                                KEEP_HEAD),
                                       directory_entry->d_name,
                                       FREE_HEAD);
            
            if (access(file_path, F_OK) + 1)
            {
                switch (size_details)
                {
                    case VERBOSE:
                        stat(file_path, &file_stat);

                        printf("%-25s %6u bytes\n", directory_entry->d_name, (unsigned int)file_stat.st_size);
                        break;
                    
                    case CONCISE:
                        printf("%s ", directory_entry->d_name);
                        break;
                }
            }

            free(file_path);   
        }

        closedir(directory_stream);     
    }

    else 
    {
        fprintf(stderr, "%s", strerror(errno));
    }
  
} 

void RestoreFile(struct Logistics *core_logistics, char *target_file, char *restore_path)
{
    //direct flow for restoring to original directory, or current/specified directory.
    


}


