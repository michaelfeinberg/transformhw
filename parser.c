#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 
Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
     line: add a line to the edge matrix - 
     takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
     ident: set the transform matrix to the identity matrix - 
     scale: create a scale matrix, 
     then multiply the transform matrix by the scale matrix - 
     takes 3 arguments (sx, sy, sz)
     translate: create a translation matrix, 
     then multiply the transform matrix by the translation matrix - 
     takes 3 arguments (tx, ty, tz)
     rotate: create an rotation matrix,
     then multiply the transform matrix by the rotation matrix -
     takes 2 arguments (axis, theta) axis should be x y or z
     apply: apply the current transformation matrix to the 
     edge matrix
     display: draw the lines of the edge matrix to the screen
     display the screen
     save: draw the lines of the edge matrix to the screen
     save the screen to a file -
     takes 1 argument (file name)
     quit: end parsing
     See the file script for an example of the file format
     IMPORTANT MATH NOTE:
     the trig functions int math.h use radian mesure, but us normal
     humans use degrees, so the file will contain degrees for rotations,
     be sure to conver those degrees to radians (M_PI is the constant
     for PI)
     ====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {
  
  FILE *f;
  char line[256];
  clear_screen(s);
  color c;
  c.red = 0;
  c.green = 250;
  c.blue = 0;
  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);
    char * temp = line;
    
    if(strcmp(line,"move") == 0){
      if( fgets(line,255,f) != NULL ){
	printf("%s",line);
	double tx,ty,tz;
	tx = atof(strsep(&temp, " "));
	ty = atof(strsep(&temp, " "));
        tz = atof(strsep(&temp, " "));
        struct matrix * translate = make_translate(tx,ty,tz);
	matrix_mult(translate, transform);
      }else{

      }      
    }else if(strcmp(line,"scale") == 0){
      if( fgets(line,255,f) != NULL ){
	printf("%s",line);
	double sx,sy,sz;
	sx = atof(strsep(&temp, " "));
	sy = atof(strsep(&temp, " "));
        sz = atof(strsep(&temp, " "));
        struct matrix * scale=  make_scale(sx,sy,sz);
	matrix_mult(scale, transform);
      }else{

      }

    }else if(strcmp(line,"rotate") == 0){
      if( fgets(line,255,f) != NULL ){
	printf("%s",line);
	char *axis;
	double theta;
	struct matrix * rotate;
	axis = strsep(&temp, " ");
	theta = atof(strsep(&temp, " "));
	if(axis[0] == 'x' || axis[0] == 'X'){
	  rotate = make_rotX(theta);
	  matrix_mult(rotate,transform);
	}else if(axis[0] == 'y' || axis[0] =='Y'){
	  rotate = make_rotY(theta);
	  matrix_mult(rotate,transform);
	}else if(axis[0] == 'z' || axis[0] =='Z'){
	  rotate = make_rotZ(theta);
	  matrix_mult(rotate,transform);
	}else{
	  printf("Input a valid rotation axis");
	  rotate = new_matrix(4,4);
	  ident(rotate);
	} 	      
      }else{

      }
    }else if(strcmp(line,"line") == 0){
      if( fgets(line,255,f) != NULL ){
	printf("%s",line);
	double x0,y0,z0,x1,y1,z1;
	x0 = atof(strsep(&temp, " "));
        y0 = atof(strsep(&temp, " "));
        z0 = atof(strsep(&temp, " "));
	x1 = atof(strsep(&temp, " "));
        y1 = atof(strsep(&temp, " "));
        z1 = atof(strsep(&temp, " "));
	add_edge(edges,x0,y0,z0,x1,y1,z1);
      }else{
	
      }
    }else if(strcmp(line,"apply") == 0){
      matrix_mult(transform,edges);
    }else if(strcmp(line,"ident") == 0){
      ident(transform);
    }else if(strcmp(line,"display") == 0){
      clear_screen(s);
      draw_lines(edges,s,c);
      display(s);
    }else if(strcmp(line,"save") == 0){
      if( fgets(line,255,f) != NULL ){
	printf("%s",line);
	
      }else{

      }

    }else{
      printf("Command does not exist");
    }
  }
}
