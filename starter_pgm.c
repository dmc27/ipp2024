
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
  // name of the input image file
  char const* const fname_inp = "fox.pgm";
  // name of the to-be-created modified image file
  char const* const fname_out = "fox.out.pgm";

  //===============================================================
  //
  // PART 1: Read the image file
  //
  //===============================================================

  FILE* p_file = fopen(fname_inp, "r");
  if (p_file == NULL)
  {
    printf("Cannot open file for reading!\n");
    return 0;
  }

  // skip whitespace/newline
  fscanf(p_file, " ");

  //-------------------------------------------------------------------------
  // Part 1A: Read the signature ("P2")
  //-------------------------------------------------------------------------
  char sig[3]; 
  fgets(sig, 3, p_file);
  printf("%s\n", sig);
  if (sig[0] != 'P' || sig[1] != '2')
  {
    printf("Invalid PGM file");
  }

  //-------------------------------------------------------------------------
  // Part 1B: Read the width, height, and maximum pixel value
  //-------------------------------------------------------------------------
  int w = -1;
  int h = -1;
  int max_val = -1;
  char line[1000];
    
  do // skip to the next valid line...
  {              
    fscanf(p_file, " "); // skip whitespace/newline        
    fgets(line, sizeof(line), p_file); // read the next line
  } 
  while (line[0] == '#'); // comment lines start with #

  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  // NOTE: At this point, the string line[] contains the last read 
  // non-comment line. Let's now try to parse this string to get
  // the width, height, and max value.  However, all three of
  // these values might not be contained in the string.  Instead,
  // the height and max value could be on later (not-yet-read) 
  // lines (both on the same line, or each on separate lines).  
  // Thus, the following code might seem a bit complicated, but it
  // basically just uses sscanf() to extract valid integer values
  // for the width, height, and max value from line[], updating
  // line[] with the next non-comment line and continuing the
  // extraction process until all three (width, height, max value)
  // values are found.
  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

  // first, assume that width, height, and max value are all
  // on the same line, and try to read them all in one shot
  int num_read = sscanf(line, "%d %d %d", &w, &h, &max_val);
  if (num_read < 3)
  {
    // we couldn't read all three, so try to read just the 
    // width and height
    num_read = sscanf(line, "%d %d", &w, &h);
    if (num_read < 2)
    {
      // we couldn't even read just the width and the height, 
      // so first read just the width
      num_read = sscanf(line, "%d", &w);
      if (num_read < 1)
      {
        printf("Invalid file format! Width not found!");
        fclose(p_file);
        return 0;
      }

      do // skip to the next valid line...
      {              
        fscanf(p_file, " "); // skip whitespace/newline        
        fgets(line, sizeof(line), p_file); // read the next line
      } 
      while (line[0] == '#'); // comment lines start with #

      // next, try to read the height and max value in one shot
      // (in the case that they are on the same line)
      num_read = sscanf(line, "%d %d", &h, &max_val);
      if (num_read < 2)
      {
        // height and max value are on different lines,
        // so first read just the height
        num_read = sscanf(line, "%d", &h);
        if (num_read < 1)
        {
          printf("Invalid file format! Height not found!");
          fclose(p_file);
          return 0;
        }

        do // skip to the next valid line...
        {              
          fscanf(p_file, " "); // skip whitespace/newline        
          fgets(line, sizeof(line), p_file); // read the next line
        } 
        while (line[0] == '#'); // comment lines start with #

        // finally, read the max value
        num_read = sscanf(line, "%d", &max_val);
        if (num_read < 1)
        {
          printf("Invalid file format! Max value not found!");
          fclose(p_file);
          return 0;
        }
      }
    }
    else // finally, read the max value
    {
      do // skip to the next valid line...
      {              
        fscanf(p_file, " "); // skip whitespace/newline        
        fgets(line, sizeof(line), p_file); // read the next line
      } 
      while (line[0] == '#'); // comment lines start with #

      // read the max value
      num_read = sscanf(line, "%d", &max_val);
      if (num_read < 1)
      {
        printf("Invalid file format! Max value not found!");
        fclose(p_file);
        return 0;
      }
    }
  }

  printf("w = %d\n", w);
  printf("h = %d\n", h);
  printf("max_val = %d\n", max_val);


  //-------------------------------------------------------------------------
  // Part 1C: Read the pixel values
  // (here using C99 variable-length 2D array)
  //-------------------------------------------------------------------------

  int img[w][h];
  
  for (int y = 0; y < h; ++y)
  {
    for (int x = 0 ; x < w; ++x)
    {
      fscanf(p_file, "%d", &img[x][y]);
    }
  }

  fclose(p_file);
  p_file = NULL;


  //===============================================================
  //
  // PART 2: Simple image adjustments
  //
  //===============================================================

  // TODO!


  //===============================================================
  //
  // PART 3: Save the output to a new PPM file
  //
  //===============================================================

  p_file = fopen(fname_out, "w");
  if (p_file == NULL)
  {
    printf("Cannot open file for writing!\n");
  }

  fprintf(p_file, "P2\n"); // write the signature
  fprintf(p_file, "%d %d\n", w, h); // write width and height
  fprintf(p_file, "%d\n", max_val); // write max value

  // write the pixel values 
  for (int y = 0; y < h; ++y)
  {
    for (int x = 0 ; x < w; ++x)
    {
      fprintf(p_file, "%d\n", img[x][y]);
    }
  }

  fclose(p_file);
  p_file = NULL;

  printf("OK!\n");

  return 0;
}