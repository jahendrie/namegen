/*******************************************************************************
 *  namegen.c   |   version 1.0     |   zlib license    |   2015-02-02
 *  James Hendrie                   |   hendrie.james@gmail.com
 *
 *      This program will generate a number of names using three included names
 *      files and according to the arguments provided by the user.
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <getopt.h>

#define VERSION "1.0"

#define FILE_FIRST_MALE "firstnames_male.txt"
#define FILE_FIRST_FEMALE "firstnames_female.txt"
#define FILE_SURNAMES "surnames.txt"

#define STRING_LENGTH 256



/*  Default options */
int names;          //  Number of names to rerieve
int useUpper;       //  Use all upper-case letters
int useLower;       //  Use all lower-case letters
int getMale;        //  Retrieve male first names
int getFemale;      //  Retrieve female first names
int getFirst;       //  Retrieve first names
int getSurname;     //  Retrieve surnames

/*
 *  OptString:
 *      h   help
 *      V   version
 *      n   number of names to retrieve
 *      u   upper-case only
 *      l   lower-case only
 *      m   male-only
 *      f   female-only
 *      s   only surnames
 *      S   supress surnames
 */
static const char *optString = "hVulmfsSn:";

/*  Data directory */
char dataDir[ (STRING_LENGTH) ];


/*
--------------------------------------------------------------------------------
                                 FIND DATA DIR
--------------------------------------------------------------------------------
 *  This locates tests a few pre-selected locations on the user's filesystem for
 *  the namegen data directory.  If none of them can be found, then the program
 *  will exit with an error.
 *
 *  Args:
 *      char *dataDir:  String pointer to which we copy the data directory
 *
 *  Returns:
 *      0   Success
 *      -1  Failure
*/
int find_data_dir( char *dataDir )
{
    /*  The directories we check */
    char dirs[ 3 ][ (STRING_LENGTH) ] = {
        "/usr/share/namegen/data",
        "/usr/local/share/namegen/data",
        "/opt/namegen/data"
    };

    /*  File pointer to test if we can read from a directory */
    FILE *fp = NULL;

    /*  Try opening each directory, return path on success */
    int i = 0;
    for( ; i < 3; ++i )
    {
        fp = fopen( dirs[ i ], "r" );
        if( fp != NULL )
        {
            fclose( fp );
            strncpy( dataDir, dirs[ i ], (STRING_LENGTH) - 1 );
            return( 0 );
        }
    }

    return( -1 );
}



/*
--------------------------------------------------------------------------------
                                   FILE ERROR
--------------------------------------------------------------------------------
 *  Generic file opening error message.
 *
 *  Args:
 *      const char *string:  Name of the file that can't be opened
*/
void file_error( const char *string )
{
    fprintf( stderr, "ERROR:  Cannot open file %s for reading.\n", string );
    fprintf( stderr, "Aborting.\n");
}


/*
--------------------------------------------------------------------------------
                                   OPEN FILE
--------------------------------------------------------------------------------
 *  Try opening a given file name and assigning the pointer to a given file
 *  pointer; the program will combine the predefined dataDir directory with the
 *  given file name to get the full path for opening.
 *
 *  Args:
 *      const char *fileName:   File name string
 *      FILE *fp:   File pointer to which we assign the opened file
 *
 *  Returns:
 *      0   Success
 *      -1  Failure
*/
int open_file( const char *fileName, FILE **fp )
{
    char path[ (STRING_LENGTH) ];
    sprintf( path, "%s/%s", dataDir, fileName );

    *fp = fopen( path, "r" );
    if( fp == NULL )
    {
        file_error( path );
        return( -1 );
    }

    return( 0 );
}


/*
--------------------------------------------------------------------------------
                                  PRINT USAGE
--------------------------------------------------------------------------------
 *  Print a brief usage message to the user's terminal
 *
 *  Args:
 *      FILE *fp:   Stream (file pointer) to which we print the message,
 *                  presumably either STDOUT or STDERR
*/
void print_usage( FILE *fp )
{
    fprintf( fp, "Usage:  namegen [OPTIONS]\n\n");
}



/*
--------------------------------------------------------------------------------
                                   PRINT HELP
--------------------------------------------------------------------------------
 *  Print some help text to the user's terminal, including a synopsis, options
 *  and a couple of examples.
*/
void print_help( void )
{
    print_usage( stdout );
    printf("This program will generate pseudo-random names for the user.\n\n");

    printf("OPTIONS:\n");
    printf("  -h        Print this help text\n");
    printf("  -V        Print version and author info\n");
    printf("  -n NUM    Print NUM names (default 1)\n");
    printf("  -u        Print names in all upper-case\n");
    printf("  -l        Print names in all lower-case\n");
    printf("  -m        Retrieve only male first names\n");
    printf("  -f        Retrieve only female first names\n");
    printf("  -s        Retrieve only surnames\n");
    printf("  -S        Do not print surnames\n\n");

    printf("Examples:\n\n");

    printf("namegen\n");
    printf("  Generate one random name, either male or female.\n");
    printf("namegen -f -n10\n");
    printf("  Print ten female names\n");
    printf("namegen -umS -n 100\n");
    printf("  Print 100 male first names, all in uppercase letters.\n");
}


/*
--------------------------------------------------------------------------------
                                 PRINT VERSION
--------------------------------------------------------------------------------
 *  Print version and author information
*/
void print_version( void )
{
    printf("namegen version %s\n", (VERSION) );
    printf("James Hendrie <hendrie.james@gmail.com>\n");
}



/*
--------------------------------------------------------------------------------
                                  COUNT NAMES
--------------------------------------------------------------------------------
 *  Go through a file and count the number of names (lines) that the file has.
 *
 *  Args:
 *      FILE *fp:  Pointer to the file we're reading
*/
int count_names( FILE *fp )
{
    /*  Init count and character buffer here */
    int names = 0;
    int ch = '\0';

    /*  Go through the file character by character */
    while( ( ch = fgetc( fp ) ) != EOF )
    {
        /*  If we hit a new line, increment the name counter */
        if( ch == '\n' )
            ++names;
    }
    
    /*  Return the number of lines counted */
    return( names );
}


/*
--------------------------------------------------------------------------------
                                   PRINT NAME
--------------------------------------------------------------------------------
 *  Print a random name from a given file
 *
 *  Args:
 *      FILE *fp:       File pointer
 *      int numLines:   The number of counted lines for that file, useful for
 *                      generating a pseudo-random number
*/
void print_name( FILE *fp, int numLines )
{
    /*  Rewind the file to the beginning */
    rewind( fp );

    /*  Seed rand again */
    srand( rand() );

    /*  Character buffer */
    char string[ (STRING_LENGTH) ];

    /*  Set our current and target positions */
    int current = 0;
    int target = rand() % numLines;     //  Random position in the file

    /*  Character */
    int ch = '\0';

    /*  Go through the file character by character */
    while( ( ch = fgetc( fp ) ) != EOF )
    {
        /*  If we've reached our mark */
        if( current == target )
        {
            /*  Move backward one byte to account for grabbed char */
            fseek( fp, -1, SEEK_CUR );

            /*  Grab the string */
            fgets( string, (STRING_LENGTH), fp );

            /*  Fix the string by removing the newline char from the end */
            string[ strlen( string ) - 1 ] = '\0';

            /*
             *  Here we further correct the string by adjusting the characters
             *  to be either upper, lower or mixed case as the user selected.
             */
            int c = 0;

            /*  Convert it all to lowercase */
            if( useLower == 1 && useUpper == 0 )
            {
                for( c = 0; c < strlen( string ); ++c )
                {
                    if( isupper( string[ c ] ) )
                        string[ c ] = tolower( string[ c ] );
                }
            }

            /*  Convert it all to uppercase */
            else if( useUpper == 1 )
            {
                for( c = 0; c < strlen( string ); ++c )
                {
                    if( islower( string[ c ] ) )
                        string[ c ] = toupper( string[ c ] );
                }
            }

            /*  Convert everything except the first character to lowercase */
            else
            {
                /*  If the first character is lowercase, capitalize it */
                if( islower( string[ 0 ] ) )
                    string[ 0 ] = toupper( string[ 0 ] );

                /*  Lowercase the rest of 'em */
                for( c = 1; c < strlen( string ); ++c )
                {
                    if( isupper( string[ c ] ) )
                            string[ c ] = tolower( string[ c ] );
                }
            }


            /*  Print the string */
            printf("%s", string );

            break;
        }

        /*  Increment current if appropriate */
        if( ch == '\n' )
            ++current;
    }
}




/*
--------------------------------------------------------------------------------
                                      MAIN
--------------------------------------------------------------------------------
 *  Main function
 *
 *  Args:
 *      argc:   Number of arguments, including the program being run
 *      argv:   The arguments themselves
 *
 *  Returns:
 *      0   Success
 *      1   Error
*/
int main( int argc, char *argv[] )
{

    /*  Seed rand with system time */
    srand( (unsigned int)time(NULL) );

    /*  Initialize options to default values */
    names = 1;          //  Number of names to rerieve
    useUpper = 0;       //  Use all upper-case letters
    useLower = 0;       //  Use all lower-case letters
    getMale = 1;        //  Retrieve male first names
    getFemale = 1;      //  Retrieve female first names
    getFirst = 1;       //  Retrieve first names
    getSurname = 1;     //  Retrieve surnames


    /*
     *
     *  ------------------------    ARGUMENT CHECKING   ------------------------
     *
     *  Using getopt to get the options because it's the bees knees, daddy-o
     */
    int opt = 0;
    opt = getopt( argc, argv, optString );
    while( opt != -1 )
    {
        switch( opt )
        {
            case 'h': print_help(); return(0); break;       //  Print help
            case 'V': print_version(); return(0); break;    //  Print version
            case 'u': useUpper = 1; useLower = 0; break;    //  All uppercase
            case 'l': useLower = 1; useUpper = 0; break;    //  All lowercase
            case 'm': getMale = 1; getFemale = 0; break;    //  Male only
            case 'f': getFemale = 1; getMale = 0; break;    //  Female only
            case 's': getSurname = 1; getFirst = 0; break;  //  Surnames only
            case 'S': getSurname = 0; getFirst = 1; break;  //  No surnames
            case 'n': names = atoi( optarg ); break;        //  No. names to get
            default:                                        //  Any other arg
                  print_usage( stderr );
                  fprintf( stderr, "See 'namegen -h' for help\n");
                  return(1);
                  break;
        }

        opt = getopt( argc, argv, optString );
    }
    /*
     *  -------------------     END ARGS    ------------------------
     */

    /*  Find the data directory */
    if( find_data_dir( dataDir ) == -1 )
    {
        fprintf( stderr, "ERROR:  Cannot open data directory for reading.\n");
        fprintf( stderr, "Aborting.\n");
        return( 1 );
    }



    /*  Create the file pointers */
    FILE *firstMale = NULL;
    FILE *firstFemale = NULL;
    FILE *surnames = NULL;

    /*  Open all the files */
    if( open_file( (FILE_FIRST_MALE), &firstMale ) == - 1 ||
            open_file( (FILE_FIRST_FEMALE), &firstFemale ) == - 1 ||
            open_file( (FILE_SURNAMES), &surnames ) == - 1 )
    {
        /*  Return 1 on failure */
        return( 1 );
    }

    /*  Count the number of names (lines) in each file */
    int numFirstMale = count_names( firstMale );
    int numFirstFemale = count_names( firstFemale );
    int numSurnames = count_names( surnames );


    /*
     *  --------    Entering the main loop ------------
     */
    int i = 0;
    for( i = 0; i < names; ++i )
    {

        /*  Get a first name if they want it */
        if( getFirst == 1 )
        {
            /*  If they want either a male or female name */
            if( getMale == 1 && getFemale == 1 )
            {
                if( rand() % 2 == 1 )
                    print_name( firstMale, numFirstMale );
                else
                    print_name( firstFemale, numFirstFemale );
            }

            /*  If they only want a male name */
            else if( getMale == 1 )
                print_name( firstMale, numFirstMale );

            /*  If they only want a female name */
            else
                print_name( firstFemale, numFirstFemale );

            /*  Put a space after the first name is printed */
            fputc( ' ', stdout );
        }

        /*  Get a surname if they want it */
        if( getSurname == 1 )
            print_name( surnames, numSurnames );

        /*  Print a newline at the end of each name retrieved */
        fputc( '\n', stdout );
    }


    /*  Close the files */
    fclose( firstMale );
    fclose( firstFemale );
    fclose( surnames );


    return(0);
}
