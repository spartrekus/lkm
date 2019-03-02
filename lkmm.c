
//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
#include <stdio.h>
#define PATH_MAX 2500
#if defined(__linux__) //linux
#define MYOS 1
#elif defined(_WIN32)
#define MYOS 2
#elif defined(_WIN64)
#define MYOS 3
#elif defined(__unix__) 
#define MYOS 4  // freebsd
#define PATH_MAX 2500
#else
#define MYOS 0
#endif
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <time.h>
#include <errno.h>

#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>  
#include <time.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define ESC "\033"
#define home() 			printf(ESC "[H") //Move cursor to the indicated row, column (origin at 1,1)
#define clrscr()		printf(ESC "[2J") //clear the screen, move to (1,1)
#define gotoxy(x,y)		printf(ESC "[%d;%dH", y, x);
#define ansigotoyx(y,x)		printf(ESC "[%d;%dH", y, x);
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

/* 
Set Display Attribute Mode	<ESC>[{attr1};...;{attrn}m
*/
#define resetcolor() printf(ESC "[0m")
#define set_display_attrib(color) 	printf(ESC "[%dm",color)


int rows, cols ; 
int pansel = 1;
char userstr[PATH_MAX];
int  mode_user_colorscheme = 0; 



void lsdir( char *foodir )
{ 
   DIR *dirp;
   struct dirent *dp;
   dirp = opendir( foodir );
   while  ((dp = readdir( dirp )) != NULL ) 
   {
         if (  strcmp( dp->d_name, "." ) != 0 )
         if (  strcmp( dp->d_name, ".." ) != 0 )
             printf( "%s\n", dp->d_name );
   }
   closedir( dirp );
}






void gfxhline( int y1 , int x1 , int x2 , int mychar )
{
    int foo, fooy , foox ;
    foo = x1;
    ansigotoyx( y1 , x1 );  
    for( foox = x1 ; foox <= x2 ; foox++) 
         printf( "%c", mychar );
}




void gfxrectangle( int y1, int x1, int y2, int x2 )
{
    int foo, fooy , foox ;
    foo = x1;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
    {
       ansigotoyx( fooy , x1 );  
       for( foox = x1 ; foox <= x2 ; foox++) 
          //mvaddch( fooy , foox , ' ' );
          printf( "." );
    }
}


void printatl( int y1, int x1,  int mylength, char *mystring )
{
    int foo, fooy , foox ;
    foo = x1;
    ansigotoyx( y1 , x1 );  
    for( foox = 0 ; foox <= mylength ; foox++) 
         printf( "%c", mystring[ foox ] );
}


void printat( int y1, int x1, char *mystring )
{
         ansigotoyx( y1 , x1 );  
         printf( "%s", mystring );
}

void mvcenter( int myposypass, char *mytext )
{
      //mvprintw( myposypass, cols/2 - strlen( mytext )/2  , "%s", mytext );
      printat( myposypass , cols/2 - strlen( mytext )/2  , mytext );
}


void gfxchar( int y1, int x1, int mychar )
{
         ansigotoyx( y1 , x1 );  
         //mvaddch( foo , foox , ACS_HLINE );
         printf( "%c", mychar );
}

void gfxframe( int y1, int x1, int y2, int x2 )
{
    int foo, fooy , foox ;
    foo = x1;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
    {
        ansigotoyx( fooy , foo );  
        //mvaddch( fooy , foo , ACS_VLINE );
        printf( "|" );
    }
    foo = x2;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
    {
         ansigotoyx( fooy , foo );  
         //mvaddch( fooy , foo , ACS_VLINE );
         printf( "|" );
    }
    foo = y1;
    for( foox = x1 ; foox <= x2 ; foox++) 
    {
         ansigotoyx( foo , foox );  
         //mvaddch( foo , foox , ACS_HLINE );
         printf( "-" );
    }
    foo = y2;
    for( foox = x1 ; foox <= x2 ; foox++) 
    {
         ansigotoyx( foo , foox );  
         //mvaddch( foo , foox , ACS_HLINE );
         printf( "-" );
    }
    gfxchar(  y1,  x1, 'O' );
    gfxchar(  y1,  x2, 'O' );
    gfxchar(  y2,  x1, 'O' );
    gfxchar(  y2,  x2, 'O' );
    //mvaddch( y1 , x1 , ACS_ULCORNER );
    //mvaddch( y1 , x2 , ACS_URCORNER );
    //mvaddch( y2 , x1 , ACS_LLCORNER );
    //mvaddch( y2 , x2 , ACS_LRCORNER );
}








void nsystem( char *mycmd )
{
   printf( "<SYSTEM>\n" );
   printf( " >> CMD:%s\n", mycmd );
   system( mycmd );
   printf( "</SYSTEM>\n");
}


void nrunwith( char *cmdapp, char *filesource )
{
           char cmdi[PATH_MAX];
           strncpy( cmdi , "  " , PATH_MAX );
           strncat( cmdi , cmdapp , PATH_MAX - strlen( cmdi ) -1 );
           strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
           strncat( cmdi , " \"" , PATH_MAX - strlen( cmdi ) -1 );
           strncat( cmdi ,  filesource , PATH_MAX - strlen( cmdi ) -1 );
           strncat( cmdi , "\" " , PATH_MAX - strlen( cmdi ) -1 );
           nsystem( cmdi ); 
}



void clear_screen_retro()
{
    int fooi;
    struct winsize w; // need ioctl and unistd 
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
    for ( fooi = 1 ; fooi <= w.ws_row ; fooi++ ) 
       printf( "\n" );
    home();
}


void ansierase()
{
    int fooi;
    struct winsize w; // need ioctl and unistd 
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
    clrscr();
    home();
}


void clear_screen()
{
    int fooi;
    struct winsize w; // need ioctl and unistd 
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
    clrscr();
    home();
}


int fexist(char *a_option)
{
  char dir1[PATH_MAX]; 
  char *dir2;
  DIR *dip;
  strncpy( dir1 , "",  PATH_MAX  );
  strncpy( dir1 , a_option,  PATH_MAX  );

  struct stat st_buf; 
  int status; 
  int fileordir = 0 ; 

  status = stat ( dir1 , &st_buf);
  if (status != 0) {
    fileordir = 0;
  }

  // this is compatible to check if a file exists
  FILE *fp2check = fopen( dir1  ,"r");
  if( fp2check ) {
  // exists
  fileordir = 1; 
  fclose(fp2check);
  } 

  if (S_ISDIR (st_buf.st_mode)) {
    fileordir = 2; 
  }
return fileordir;
/////////////////////////////
}




///////////////////////////////////////////
void readfile( char *filesource )
{
   FILE *source; 
   int ch ; 
   source = fopen( filesource , "r");
   if ( source == NULL ) { printf( "File not found.\n" ); } else {
   while( ( ch = fgetc(source) ) != EOF )
   {
         printf( "%c", ch );
   }
   fclose(source);
   }
}




static struct termios oldt;

void restore_terminal_settings(void)
{
    tcsetattr(0, TCSANOW, &oldt);  /* Apply saved settings */
}

void enable_waiting_for_enter(void)
{
    tcsetattr(0, TCSANOW, &oldt);  /* Apply saved settings */
}

void disable_waiting_for_enter(void)
{
    struct termios newt;

    /* Make terminal read 1 char at a time */
    tcgetattr(0, &oldt);  /* Save terminal settings */
    newt = oldt;  /* Init new settings */
    newt.c_lflag &= ~(ICANON | ECHO);  /* Change settings */
    tcsetattr(0, TCSANOW, &newt);  /* Apply settings */
    atexit(restore_terminal_settings); /* Make sure settings will be restored when program ends  */
}





void printtypes( char *mydir )
{
   DIR *dirp; 
   struct dirent *dp;
   dirp = opendir( mydir  );
   while  ((dp = readdir( dirp )) != NULL ) 
   {
        printf("%s", KNRM );
        //if ( entrycounter <= nexp_user_scrolly[panviewpr] )
        //      continue;
        //if ( dp->d_type == DT_DIR ) 
        printf( "%d %s\n" , dp->d_type, dp->d_name ) ;
   }
   closedir( dirp );
}



/////////////////////////
/////////////////////////
int  nexp_user_sel[5] ; 
int  nexp_user_scrolly[5] ;
char nexp_user_fileselection[PATH_MAX]; 
int  tc_det_dir_type = 0;
/////////////////////////
void printdir( int pyy, int fopxx, char *mydir , int panviewpr )
{
   int pxx = fopxx;
   if ( pxx == 0 )       pxx = 2;
   DIR *dirp; int posy = 0;  int posx, chr ; 
   int fooselection = 0;
   posy = 1; posx = cols/2;
   char cwd[PATH_MAX];
   char path[PATH_MAX];
   struct dirent *dp;
   dirp = opendir( mydir  );
   int entrycounter = 0;
   fooselection = 0;

   struct stat info;

   while ((dp = readdir(dirp)) != NULL) 
   {
      if ( posy <= rows-3 )
      { 

      entrycounter++;
      if ( entrycounter <= nexp_user_scrolly[panviewpr] )
              continue;

      if (dp->d_name[0] != '.') 
      {
        printf("%s", KNRM);
        posy++; fooselection++;

        strcpy(path, mydir );
        strcat(path, "/");
        strcat(path, dp->d_name);

        if (stat(path, &info) != 0)
          fprintf(stderr, "stat() error on %s: %s\n", path, strerror(errno));

        else if (S_ISDIR(info.st_mode))
        {
               //traverse(path, indent+1);
                printf("%s", KYEL);
        }

            ansigotoyx( posy, pxx-1 );
            if ( nexp_user_sel[ panviewpr ] == fooselection ) 
            {
                  if ( panviewpr == pansel )
                  {
                    strncpy( nexp_user_fileselection, dp->d_name , PATH_MAX );
                    //color_set( 4, NULL );
                    printf( ">" );
                  }
            }
            else 
                  printf( " " );


            ansigotoyx( posy, pxx );
            for ( chr = 0 ;  chr <= strlen(dp->d_name) ; chr++) 
            {
              if  ( dp->d_name[chr] == '\n' )
              {    //posx = cols/2;
              }
              else if  ( dp->d_name[chr] == '\0' )
              {    //posx = cols/2;
              }
              else
              {  
                 //mvaddch( posy, posx++ , dp->d_name[chr] );
                 printf( "%c", dp->d_name[chr] );
                 posx++;
              }
            }

         }
       }
   }
   printf("%s", KNRM );
}




////////////////////////////////////////////////////////////////////
char *strcut( char *str , int myposstart, int myposend )
{     // copyleft, C function made by Spartrekus 
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( ( str[i] != '\0' ) && ( str[i] != '\0') )
         if ( ( i >=  myposstart-1 ) && (  i <= myposend-1 ) )
           ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}






/////////////////////////////////////
void strninput( char *mytitle, char *foostr )
{
      strncpy( userstr , "" , PATH_MAX );
      disable_waiting_for_enter();
      char strmsg[PATH_MAX];
      char charo[PATH_MAX];
      strncpy( strmsg, foostr , PATH_MAX );
      int foousergam = 0; int ch ;  int chr;
      while( foousergam == 0 ) 
      {

         home();
         for ( chr = 0 ;  chr <= cols-1 ; chr++) printf( " ");

         home();
         printf( "|%s|", mytitle );

         ansigotoyx( rows, 0 );
         for ( chr = 0 ;  chr <= cols-1 ; chr++) printf( " ");
         ansigotoyx( rows, 0 );
         printf( ": %s", strmsg );

         ch = getchar();
         if ( ch == 10 )       foousergam = 1;

	 else if ( ch == 27 ) 
         {
              foousergam = 1;
	      strncpy( strmsg, ""  ,  PATH_MAX );
         }

         else if ( ch == 127 ) 
            strncpy( strmsg, strcut( strmsg, 1 , strlen( strmsg ) -1 )  ,  PATH_MAX );

         else if ( ch == 8 ) 
            strncpy( strmsg, strcut( strmsg, 1 , strlen( strmsg ) -1 )  ,  PATH_MAX );

	 else if (
			(( ch >= 'a' ) && ( ch <= 'z' ) ) 
		        || (( ch >= 'A' ) && ( ch <= 'Z' ) ) 
		        || (( ch >= '1' ) && ( ch <= '9' ) ) 
		        || (( ch == '0' ) ) 
		        || (( ch == '~' ) ) 
		        || (( ch == '!' ) ) 
		        || (( ch == '&' ) ) 
		        || (( ch == '=' ) ) 
		        || (( ch == ':' ) ) 
		        || (( ch == ';' ) ) 
		        || (( ch == '<' ) ) 
		        || (( ch == '>' ) ) 
		        || (( ch == ' ' ) ) 
		        || (( ch == '|' ) ) 
		        || (( ch == '#' ) ) 
		        || (( ch == '?' ) ) 
		        || (( ch == '+' ) ) 
		        || (( ch == '/' ) ) 
		        || (( ch == '\\' ) ) 
		        || (( ch == '.' ) ) 
		        || (( ch == '$' ) ) 
		        || (( ch == '%' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == ',' ) ) 
		        || (( ch == '{' ) ) 
		        || (( ch == '}' ) ) 
		        || (( ch == '(' ) ) 
		        || (( ch == ')' ) ) 
		        || (( ch == ']' ) ) 
		        || (( ch == '[' ) ) 
		        || (( ch == '*' ) ) 
		        || (( ch == '"' ) ) 
		        || (( ch == '@' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == '_' ) ) 
		        || (( ch == '^' ) ) 
		        || (( ch == '\'' ) ) 
	             ) 
		  {
                        snprintf( charo, PATH_MAX , "%s%c",  strmsg, ch );
		        strncpy( strmsg,  charo ,  PATH_MAX );
		  }
     }
     strncpy( userstr, strmsg , PATH_MAX );
}






/////////////////////////////////////
void traverse(char *fn, int indent) {
  DIR *dir;
  struct dirent *entry;
  int count;
  char path[1025]; /*** EXTRA STORAGE MAY BE NEEDED ***/
  struct stat info;

  for (count=0; count<indent; count++) printf("  ");
  printf("%s\n", fn);

  if ((dir = opendir(fn)) == NULL)
    perror("opendir() error");
  else {
    while ((entry = readdir(dir)) != NULL) 
    {
      if (entry->d_name[0] != '.') 
      {

        strcpy(path, fn);
        strcat(path, "/");
        strcat(path, entry->d_name);

        if (stat(path, &info) != 0)
          fprintf(stderr, "stat() error on %s: %s\n", path, strerror(errno));

        else if (S_ISDIR(info.st_mode))
               traverse(path, indent+1);
      }
    }
    closedir(dir);
  }
}





int main( int argc, char *argv[])
{

    ////////////////////////////////////////////////////////
    char cwd[PATH_MAX];
    char pathbefore[PATH_MAX];
    char pathpile[PATH_MAX];
    strncpy( pathbefore , getcwd( cwd, PATH_MAX ) , PATH_MAX );
    strncpy( pathpile , getcwd( cwd, PATH_MAX ) , PATH_MAX );
    char pathpan[5][PATH_MAX];
    char string[PATH_MAX];
    char cmdi[PATH_MAX];
    char clipboard_path[PATH_MAX];
    int fooi;
    int foo;
    ////////////////////////////////////////////////////////
    if ( argc == 2)
    if ( strcmp( argv[1] , "" ) !=  0 )
    if ( fexist( argv[1] ) ==  2 )
    {
          chdir( argv[ 1 ] );
          strncpy( pathpan[ 1 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
          strncpy( pathpan[ 2 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
          strncpy( pathpan[ 3 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
          strncpy( pathpan[ 4 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
    }



    ////////////////////////////////////////////////////////
    int viewpan[5];
    nexp_user_sel[1] = 1;
    nexp_user_sel[2] = 1;
    nexp_user_scrolly[1] = 0;
    nexp_user_scrolly[2] = 0;
    nexp_user_sel[3] = 1;
    nexp_user_sel[4] = 1;
    nexp_user_scrolly[3] = 0;
    nexp_user_scrolly[4] = 0;
    strncpy( pathpan[ 1 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
    strncpy( pathpan[ 2 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
    strncpy( pathpan[ 3 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
    strncpy( pathpan[ 4 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
    viewpan[ 1 ] = 1;
    viewpan[ 2 ] = 1;
    viewpan[ 3 ] = 0;
    viewpan[ 4 ] = 0;

    struct winsize w; // need ioctl and unistd 
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
    ////////////////////////////////////////////////////////
    if ( argc == 2)
    if ( strcmp( argv[1] , "-s" ) ==  0 ) 
    {
       printf("Screen\n" );
       printf("Env HOME:  %s\n", getenv( "HOME" ));
       printf("Env PATH:  %s\n", getcwd( string, PATH_MAX ) );
       printf("Env TERM ROW:  %d\n", w.ws_row );
       printf("Env TERM COL:  %d\n", w.ws_col );
       return 0;
    }
    rows = w.ws_row ; 
    cols = w.ws_col ; 



    ////////////////////////////////////////////////////////
    if ( argc == 3)
    if ( strcmp( argv[1] , "-f" ) ==  0 ) 
    {
       printf("%s", KYEL);
       printf("%syellow\n", KYEL);
       readfile( argv[ 2 ] );
       return 0;
    }
   
    ////////////////////////////////////////////////////////
    if ( argc == 2)
    {
       printf("%syellow\n", KYEL);
       printf( "PATH1:%s\n",  pathpan[ 1 ] );
       printf( "PATH2:%s\n",  pathpan[ 2 ] );
       //return 0;
    }
    int ch ; 
    int chr; 
    int gameover = 0;

    while ( gameover == 0 ) 
    {
       strncpy( nexp_user_fileselection, "" , PATH_MAX );
       disable_waiting_for_enter();

       // not enabled to get faster
       //if      ( mode_user_colorscheme == 1 )
       //   printf("%syellow\n", KYEL);
       //else if ( mode_user_colorscheme == 2 )
       //   printf("%sgreen\n", KGRN);

       clear_screen();


       ansigotoyx( 1, 0 );
       for ( chr = 0 ;  chr <= cols-1 ; chr++) printf( " ");
       ansigotoyx( 2, 0 );
       for ( chr = 0 ;  chr <= cols-1 ; chr++) printf( " ");

       //ansigotoyx( rows, 0 );
       //gfxhline(  0 , 0 , cols-1 , ' ' ); 
       //gfxhline(  1 , 0 , cols-1 , ' ' ); 

       if ( viewpan[ 1 ] == 1 ) 
       if ( viewpan[ 3 ] == 0 ) 
       {
          //snprintf( string , PATH_MAX , "| 1 |[%s]", pathpan[  1  ] );
          strncpy( string, "| 1 |[" , PATH_MAX );
          strncat( string , pathpan[ 1 ] , PATH_MAX - strlen( string ) - 1);
          strncat( string , "]" , PATH_MAX - strlen( string ) - 1);
          printatl( 0 , 0 ,  cols / 2 - 4, string );
          chdir( pathpan[ 1 ] );
          printdir( 0, 0, "." , 1 );
       }

       if ( viewpan[ 2 ] == 1 ) 
       if ( viewpan[ 4 ] == 0 ) 
       {
          //ansigotoyx( 0, cols/2 );
          //printf( "| 2 |[%s]", pathpan[ 2 ] );
          //snprintf( string , PATH_MAX , "| 2 |[%s]", pathpan[  2  ] );
          strncpy( string, "| 2 |[" , PATH_MAX );
          strncat( string , pathpan[ 2 ] , PATH_MAX - strlen( string ) - 1);
          strncat( string , "]" , PATH_MAX - strlen( string ) - 1);
          printatl( 0 , cols/2 ,  cols / 2 - 4, string );
          chdir( pathpan[ 2 ] );
          printdir( 0, cols/2,  "." , 2 );
       }

       if ( viewpan[ 3 ] == 1 ) 
       {
          ansigotoyx( 0, 0 );
          printf( "| 3 |[%s]", pathpan[ 3 ] );
          chdir( pathpan[ 3 ] );
          printdir( 0, 0,      "." , 3 );
       }

       if ( viewpan[ 4 ] == 1 ) 
       {
          ansigotoyx( 0, cols/2 );
          printf( "| 4 |[%s]", pathpan[ 4 ] );
          chdir( pathpan[ 4 ] );
          printdir( 0, cols/2,  "." , 4 );
       }

       ansigotoyx( rows-1, 0 );
       printf( "|P%d|F|%d|[%s]", pansel, nexp_user_sel[pansel] ,  nexp_user_fileselection );

       ansigotoyx( rows, 0 );
       printf( "|(hjkl):Select| |(c-o):chdir| |(c-f):File Op.| |(Q)uit| |(q)uick menu|" );

       ch = getchar();

       chdir( pathpan[ pansel ] );

       if ( ch == 'y')      
       {
            chdir( pathpan[ pansel ] );
            strncpy( clipboard_path , getcwd( cwd, PATH_MAX ), PATH_MAX );
            strncpy( pathpile , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }

       else if ( ch == 'm')      
       {
            chdir( pathpan[ pansel ] );
            strncpy( pathpile , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }

       else if ( ( ch == '#')  || ( ch == '\'')  || ( ch == 'p' ) )
       {
            chdir( pathpan[ pansel ] );
            chdir( pathpile );
            strncpy( pathpan[ pansel ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
            nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
       }

       else if ( ch == 'w')      
       {
            chdir( pathpan[ pansel ] );
            chdir( getenv( "HOME" ) );
            chdir( "workspace" );
            nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
            strncpy( pathpan[ pansel ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }
       else if ( ch == '~')      
       {
            chdir( pathpan[ pansel ] );
            chdir( getenv( "HOME" ) );
            nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
            strncpy( pathpan[ pansel ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }
       else if ( ch == 'h')      
       {
            chdir( pathpan[ pansel ] );
            chdir( ".." );
            nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
            strncpy( pathpan[ pansel ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }
       else if ( ch == 'l')      
       {
            chdir( pathpan[ pansel ] );
            chdir( nexp_user_fileselection );
            nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
            strncpy( pathpan[ pansel ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }
       else if ( ( ch == 'o') && ( pansel == 1 )   )
       {
            chdir( pathpan[ 1 ] );
            chdir( nexp_user_fileselection );
            nexp_user_sel[ 2 ] = 1; 
            nexp_user_scrolly[ 2 ] = 0; 
            strncpy( pathpan[ 2 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }
       else if ( ( ch == 'o') && ( pansel == 2 )   )
       {
            chdir( pathpan[ 2 ] );
            chdir( nexp_user_fileselection );
            nexp_user_sel[ 1 ] = 1; 
            nexp_user_scrolly[ 1 ] = 0; 
            strncpy( pathpan[ 1 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }
       else if ( ( ch == 'o') && ( pansel == 3 )   )
       {
            chdir( pathpan[ 3 ] );
            chdir( nexp_user_fileselection );
            nexp_user_sel[ 4 ] = 1; 
            nexp_user_scrolly[ 4 ] = 0; 
            strncpy( pathpan[ 4 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }
       else if ( ( ch == 'o') && ( pansel == 4 )   )
       {
            chdir( pathpan[ 4 ] );
            chdir( nexp_user_fileselection );
            nexp_user_sel[ 3 ] = 1; 
            nexp_user_scrolly[ 3 ] = 0; 
            strncpy( pathpan[ 3 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }
       else if ( ch == 'k')      nexp_user_sel[pansel]--;
       else if ( ch == 'j')      nexp_user_sel[pansel]++;
       else if ( ch == 'g')      { nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; }
       else if ( ch == 'u')      nexp_user_scrolly[pansel]-=4;
       else if ( ch == 'd')      nexp_user_scrolly[pansel]+=4;
       else if ( ch == 'n')      nexp_user_scrolly[pansel]+=4;


       else if ( ch == 't' )
       { if (  mode_user_colorscheme == 1 ) mode_user_colorscheme = 0 ; else mode_user_colorscheme = 1 ;
         set_display_attrib( 0 );
       }

       else if ( ch == 'T' )
       { if (  mode_user_colorscheme == 2 ) mode_user_colorscheme = 0 ; else mode_user_colorscheme = 2 ;
         set_display_attrib( 0 );
       }

       else if ( ch == 'C' )     printf("%scyan\n",   KCYN);



       else if ( ch == 246 )   {  enable_waiting_for_enter();  nrunwith(  " tcview ",  nexp_user_fileselection    );   }
       else if ( ch == 'r' )   {  enable_waiting_for_enter();  nrunwith(  " tcview ",  nexp_user_fileselection    );   }
       else if ( ch == 'v' )   {  enable_waiting_for_enter();  nrunwith(  " vim  ",  nexp_user_fileselection    );   }
       else if ( ch == 'z' )   {  enable_waiting_for_enter();  nrunwith(  " less  ",  nexp_user_fileselection    );   }
       else if ( ch == '0' )
       {  viewpan[ 1 ] = 0;  viewpan[ 2 ] = 0; 
          viewpan[ 3 ] = 0;  viewpan[ 4 ] = 0; 
          pansel = 0; 
       }

       else if ( ch == '1' )
       {  if ( viewpan[ 1 ] == 1 )   viewpan[ 1 ] = 0; else viewpan[ 1 ] = 1; 
          pansel = 1;
       }
       else if ( ch == '2' )
       { if ( viewpan[ 2 ] == 1 )    viewpan[ 2 ] = 0; else viewpan[ 2 ] = 1; 
         pansel = 2;
       }
       else if ( ch == '3' )
       { if ( viewpan[ 3 ] == 1 )    viewpan[ 3 ] = 0; else viewpan[ 3 ] = 1; 
         pansel = 3;
       }
       else if ( ch == '4' )
       { if ( viewpan[ 4 ] == 1 )    viewpan[ 4 ] = 0; else viewpan[ 4 ] = 1; 
         pansel = 4;
       }

       else if ( ( ch == 'i' ) || ( ch == 9 )) 
       {  if      ( pansel == 1 )  
             pansel = 2 ; 
          else if ( pansel == 2 )  
             pansel = 1 ; 
          else if ( pansel == 3 )  
             pansel = 4 ; 
          else if ( pansel == 4 )  
             pansel = 3 ; 
          else 
            pansel = 1; 
          viewpan[ pansel ] = 1;
       }

       else if ( ch == '?') 
       {
            enable_waiting_for_enter();
            clear_screen_retro();
            printf( "======= \n" );
            printf( "= LFM = \n" );
            printf( "======= \n" );
            printf( " \n" );
            printf( "LFM is a twin pan file manager, for FreeBSD, NetBSD, and Linux.\n" );
            printf( " \n" );
            printf( " Author: Spartrekus\n" );
            printf( " Author OS: NetBSD\n" );
            disable_waiting_for_enter();
            getchar();
        }




       else if ( ch == 'Q') 
       {
            set_display_attrib( 7 );
            gfxrectangle( rows*30/100 , cols*30/100, rows*70/100, cols*70/100 );
            gfxframe(     rows*30/100 , cols*30/100, rows*70/100, cols*70/100 );
            mvcenter(     rows*30/100, "|MENU FILE|");
            fooi = 1;
            printat(   rows*30/100 +fooi++ , cols*30/100+1 , "y: Quit ");
            printat(   rows*30/100 +fooi++ , cols*30/100+1 , "n: Abort ");
            ansigotoyx(  rows*70/100 , cols*70/100 );
            ch = getchar();
            set_display_attrib( 0 );
            ansigotoyx( rows-1, 0 );
            if           ( ch == 'y' ) gameover = 1;
            else if      ( ch == 'Y' ) gameover = 1;
            ch = 0;
            set_display_attrib( 0 );
        }



       else if ( ch == 'q') 
       {
            set_display_attrib( 7 );
            gfxrectangle( rows*30/100 , cols*30/100, rows*70/100, cols*70/100 );
            gfxframe(     rows*30/100 , cols*30/100, rows*70/100, cols*70/100 );
            mvcenter(     rows*30/100, "|QUICK APPS|");
            fooi = 1;
            printat(   rows*30/100 +fooi++ , cols*30/100+1 , "1: naclock ");
            printat(   rows*30/100 +fooi++ , cols*30/100+1 , "2: nclock ");
            printat(   rows*30/100 +fooi++ , cols*30/100+1 , "3: vim ");
            printat(   rows*30/100 +fooi++ , cols*30/100+1 , "4: ndesk ");
            printat(   rows*30/100 +fooi++ , cols*30/100+1 , "5: links ");
            printat(   rows*30/100 +fooi++ , cols*30/100+1 , "----------");
            printat(   rows*30/100 +fooi++ , cols*30/100+1 , "w: workspace ");
            printat(   rows*30/100 +fooi++ , cols*30/100+1 , "e: wdisk ");

            ansigotoyx(  rows*70/100 , cols*70/100 );
            ch = getchar();
            set_display_attrib( 0 );
            ansigotoyx( rows-1, 0 );
            if      ( ch == '1' ) nsystem( " naclock  " ); 
            else if ( ch == '2' ) nsystem( " nclock    " ); 
            else if ( ch == '3' ) nsystem( " vim    " ); 
            else if ( ch == '4' ) nsystem( " ndesk    " ); 
            else if ( ch == '5' ) nsystem( " links www.duckduckgo.com " ); 
            else if ( ch == 'w' ) 
            {
                      chdir( pathpan[ pansel ] );
                      chdir( getenv( "HOME"  ) );
                      chdir( "workspace" );
                      nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
                      strncpy( pathpan[ pansel ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
            }
            else if ( ch == 'e' ) 
            {
                      chdir( pathpan[ pansel ] );
                      chdir( getenv( "HOME"  ) );
                      chdir( "workspace" );
                      chdir( "wdisk" );
                      nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
                      strncpy( pathpan[ pansel ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
            }
            ch = 0;
            set_display_attrib( 0 );
        }

       else if ( ch == '/') 
       {
            strninput( "Search:", "" );
            strncpy( string, userstr , PATH_MAX );
            printf("\n" );
            printf("\n" );
            printf("got: \"%s\"\n", string );
            if ( strcmp( string, "lkmm" ) == 0 )  
            {
            }
       }

       else if ( ( ch == 's') ||  ( ch == 'f')  )
       {
            enable_waiting_for_enter();
            clear_screen_retro();
            printf( "======= \n" );
            printf( "= LFM = \n" );
            printf( "======= \n" );
            printf( " \n" );
            printf( " \n" );
            printf("PATH:  %s \n", getcwd( cwd, PATH_MAX) );
            printf( " \n" );
            printf("PATH1: %s \n", pathpan[ 1 ] );
            printf( " \n" );
            printf("PATH2: %s \n", pathpan[ 2 ] );
            printf( " \n" );
            printf("PATH3: %s \n", pathpan[ 3 ] );
            printf( " \n" );
            printf("PATH4: %s \n", pathpan[ 4 ] );
            printf( " \n" );

            printf("|PATH PILE: %s|", pathpile );
            printf( "\n" );
            printf( "\n" );
            printf("|PATH CLIP: %s|", clipboard_path );
            printf( "\n" );
            //////////////////
            ansigotoyx( rows-1, 0 );
            for ( chr = 0 ;  chr <= cols-1 ; chr++) printf( " ");
            ansigotoyx( rows-1, 0 ); printf( "<Press Key>" );
            disable_waiting_for_enter();
            getchar();
            //////////////////
        }

        else if ( (ch == 127 )  || (ch == 27 ) )
        {
                 printf( "Getchar\n" ); getchar();
                 printf( "Getchar\n" ); getchar();
        }


        else if (ch == ':') 
        {
            strninput( "CMD:", "" );
            strncpy( string, userstr , PATH_MAX );
            printf("\n" );
            printf("\n" );
            printf("got: \"%s\"\n", string );
            if ( strcmp( string, "lkmm" ) == 0 )  
            {
                 nsystem( " lkmm " );
            }

            else if ( strcmp( string, "ls" ) == 0 )  
            {
                 chdir( pathpan[ pansel ] ); 
                 clear_screen();
                 printf( "LSDIR:%s\n", pathpan[ pansel ] );
                 lsdir( pathpan[ pansel ] );
            }

            else if ( strcmp( string, "type" ) == 0 )  
            {
                 printtypes( pathpan[ pansel ] ); 
                 getchar();
            }

            else if ( strcmp( string, "tra" ) == 0 )  
            {
                 traverse( "/etc", 0 );
                 getchar();
            }

            else if ( strcmp( string, "key" ) == 0 )  
            {
                 ch = getchar(); printf( "\nKEY %d %c\n", ch , ch );
                 ch = getchar(); printf( "\nKEY %d %c\n", ch , ch );
                 ch = getchar(); printf( "\nKEY %d %c\n", ch , ch );
                 ch = getchar(); printf( "\nKEY %d %c\n", ch , ch );
                 ch = getchar(); printf( "\nKEY %d %c\n", ch , ch );
                 ch = getchar(); printf( "\nKEY %d %c\n", ch , ch );
                 ch = getchar(); printf( "\nKEY %d %c\n", ch , ch );
                 ch = getchar(); printf( "\nKEY %d %c\n", ch , ch );
            }
            else if ( strcmp( string, "rect" ) == 0 )  
            {
               gfxrectangle( rows*30/100 , cols*30/100, rows*70/100, cols*70/100 );
            }
            printf( "<Press Key>\n" );
            getchar();
        }

        else if ( ch == 6 ) 
        {
            set_display_attrib( 7 );
            gfxrectangle( rows*30/100 , cols*30/100, rows*70/100, cols*70/100 );
            gfxframe(     rows*30/100 , cols*30/100, rows*70/100, cols*70/100 );
            mvcenter(     rows*30/100, "|MENU FILE OP.|");
            set_display_attrib( 0 );
            getchar();
        }

        else if ( ch == 15 ) 
        {
           strninput( " Change Directory (chdir) ", "" );
           strncpy( string, userstr , PATH_MAX );
           printf("\n" );
           printf("\n" );
           printf("got: \"%s\"\n", string );
           if ( strcmp( string , "" ) != 0 )
           {
               chdir( pathpan[ pansel ] );
               chdir( string ) ; 
               nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
               strncpy( pathpan[ pansel ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
           }
        }

        else if (ch == '7') 
        {
            strninput( " Create Directory (MKDIR) ", "" );
            strncpy( string, userstr , PATH_MAX );
            printf("\n" );
            printf("\n" );
            printf("got: \"%s\"\n", string );
            if ( strcmp( string , "" ) != 0 ) nrunwith( " mkdir " , string ) ; 
        }

        else if ( ch == '5') 
        {
                   chdir( pathpan[ pansel ] );
                   strncpy( cmdi, " cp   " , PATH_MAX );
                   strncat( cmdi , " \"" , PATH_MAX - strlen(cmdi) - 1);
                   strncat( cmdi ,   nexp_user_fileselection  , PATH_MAX - strlen(cmdi) - 1);
                   strncat( cmdi , "\" " , PATH_MAX - strlen(cmdi) - 1);
                   strncat( cmdi , "  " , PATH_MAX - strlen(cmdi) - 1);
                   if ( pansel == 2 ) foo = 1; else if ( pansel == 1 ) foo = 2; 
                   strncat( cmdi , " \"" , PATH_MAX - strlen(cmdi) - 1);
                   strncat( cmdi , pathpan[ foo ] ,  PATH_MAX - strlen(cmdi) - 1);
                   strncat( cmdi , "\" " , PATH_MAX - strlen(cmdi) - 1);
                   ansigotoyx( rows, 0 );
                   gfxhline( rows , 0 , cols-1, ' '); 
                   ansigotoyx( rows-1, 0 );
                   gfxhline( rows-1 , 0 , cols-1 , ' ' ); 
                   ansigotoyx( rows, 0 );
                   gfxhline(  rows-1 , 0 , cols-1 , '=' ); 
                   printf( "CMD: %s [y/n]?\n" ,  cmdi );
                   printf( "Answer: Yes or No [y/n]?\n" );
                   printf( "=========================\n" );
                   foo = getchar();
                   if ( ( foo == '1' ) || ( foo == 'y' ) )
                      nsystem( cmdi );
        }



        else if ( ch == '6') 
        {
                   chdir( pathpan[ pansel ] );
                   strncpy( cmdi, " mv   " , PATH_MAX );
                   strncat( cmdi , " \"" , PATH_MAX - strlen(cmdi) - 1);
                   strncat( cmdi ,   nexp_user_fileselection  , PATH_MAX - strlen(cmdi) - 1);
                   strncat( cmdi , "\" " , PATH_MAX - strlen(cmdi) - 1);
                   strncat( cmdi , "  " , PATH_MAX - strlen(cmdi) - 1);
                   if ( pansel == 2 ) foo = 1; else if ( pansel == 1 ) foo = 2; 
                   strncat( cmdi , " \"" , PATH_MAX - strlen(cmdi) - 1);
                   strncat( cmdi , pathpan[ foo ] ,  PATH_MAX - strlen(cmdi) - 1);
                   strncat( cmdi , "\" " , PATH_MAX - strlen(cmdi) - 1);
                   ansigotoyx( rows, 0 );
                   gfxhline( rows , 0 , cols-1, ' '); 
                   ansigotoyx( rows-1, 0 );
                   gfxhline( rows-1 , 0 , cols-1 , ' ' ); 
                   ansigotoyx( rows, 0 );
                   gfxhline(  rows-1 , 0 , cols-1 , '=' ); 
                   printf( "CMD: %s [y/n]?\n" ,  cmdi );
                   printf( "Answer: Yes or No [y/n]?\n" );
                   printf( "=========================\n" );
                   foo = getchar();
                   if ( ( foo == '1' ) || ( foo == 'y' ) )
                      nsystem( cmdi );
        }





        else if (ch == '!') 
        {
            strninput( " Run Cmd on File (!) ", "" );
            strncpy( string, userstr , PATH_MAX );
            printf("\n" );
            printf("\n" );
            printf("got: \"%s\"\n", string );
            if ( strcmp( string , "" ) != 0 ) 
               nrunwith( string , nexp_user_fileselection  ) ; 
        }

        else if (ch == '"') 
        {
            strninput( " Run Cmd on File (!) ", "" );
            strncpy( string, userstr , PATH_MAX );
            printf("\n" );
            printf("\n" );
            printf("got: \"%s\"\n", string );
            if ( strcmp( string , "" ) != 0 ) 
            { 
               nrunwith( string , nexp_user_fileselection  ) ; 
               //////////////////
               ansigotoyx( rows-1, 0 );
               for ( chr = 0 ;  chr <= cols-1 ; chr++) printf( " ");
               ansigotoyx( rows-1, 0 ); printf( "<Press Key>" );
               disable_waiting_for_enter();
               getchar();
               //////////////////
            }
        }

       else if ( ch == '$' )  // run sh command
        {
            strninput( " Run System Command ", "" );
            strncpy( string, userstr , PATH_MAX );
            printf("\n" );
            printf("\n" );
            printf("got: \"%s\"\n", string );
            if ( strcmp( string , "" ) != 0 ) 
            {   nsystem( string );
            //////////////////
            ansigotoyx( rows-1, 0 );
            for ( chr = 0 ;  chr <= cols-1 ; chr++) printf( " ");
            ansigotoyx( rows-1, 0 ); printf( "<Press Key>" );
            disable_waiting_for_enter();
            getchar();
            //////////////////
            }
        }

    }

    enable_waiting_for_enter();
    printf( "\n" );
    printf( "Bye!\n" );
    return 0;
}


