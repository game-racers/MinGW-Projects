//g++ -o bayerAssignment.exe bayerAssignment.cpp -O2 -lgdi32

#include <iostream>
#include "CImg/CImg.h"

using namespace cimg_library;
using namespace std;

int main() {
  CImg<unsigned char> orig("boeing.bmp");
  //Create empty image to fill in with colored Bayer Filter
  CImg<unsigned char> bayerFilter(orig.width(), orig.height(), 1, 3, 0);
  CImg<unsigned char> finalBayer(orig.width(), orig.height(), 1, 3, 0);
  
  //Copy over green intensities (every other pixel)
  for (int i = 0; i < orig.width(); i++){
	  for(int j = i%2; j < orig.height(); j+=2){
		 //Copy over green channel intensity for this pixel 
		 bayerFilter(i,j,0,1) = orig(i,j,0,1); 
		 //red and blue are zero
		 bayerFilter(i,j,0,0) = 0.0;
		 bayerFilter(i,j,0,2) = 0.0;
	  }
  }
  
  //Copy over blue intensities (every other pixel of every other row)
  //Fill in here//
  for (int i = 0; i < orig.width(); i += 2) {
	  for (int j = 1; j < orig.height(); j += 2) {
		  //Copy over the blue channel intensity for this pixel
		  bayerFilter(i, j, 0, 2) = orig(i, j, 0, 2);
		  //red and green are zero
		  bayerFilter(i, j, 0, 0) = 0.0;
		  bayerFilter(i, j, 0, 1) = 0.0;
	  }
  }
  //Copy over red intensities (every other pixel of every other row)
  //Fill in here//  
  for (int i = 1; i < orig.width(); i += 2) {
	  for (int j = 0; j < orig.height(); j += 2) {
		  //Copy over the red channel intensity for this pixel
		  bayerFilter(i, j, 0, 0) = orig(i, j, 0, 0);
		  //blue and green are zero
		  bayerFilter(i, j, 0, 1) = 0.0;
		  bayerFilter(i, j, 0, 2) = 0.0;
	  }
  }
  
  //Display and save filtered image
  CImgDisplay disp1(bayerFilter); 
  while (!disp1.is_closed())
      disp1.wait(); 
  
  bayerFilter.save("bayerFilter.bmp");
  
  //Reconstruct image from filter
  for (int i = 0; i < orig.width(); i++) {
	  for (int j = 0; j < orig.height(); j++) {
			//GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG for all Green values, add the red values and divide by 2 and same with blue values. 
		  if (i % 2 == j % 2) {
													// Red values, to the left and right of G
			  if (i == 0)									//Left Side
				  finalBayer(i, j, 0, 0) = bayerFilter(i+1, j, 0, 0);
			  else if (i+1 >= orig.width())				    //right side
				  finalBayer(i, j, 0, 0) = bayerFilter(i-1, j, 0, 0);
			  else {										//middle
				  if (j % 2 == 1)
					  finalBayer(i, j, 0, 0) = (bayerFilter(i, j + 1, 0, 0) + bayerFilter(i, j - 1, 0, 0)) / 2;
				  else											// For alternating rows, above and below is upper line and side to side is the lower line.
					  finalBayer(i, j, 0, 0) = (bayerFilter(i + 1, j, 0, 0) + bayerFilter(i - 1, j, 0, 0)) / 2;
			  }

													// Blue values, up and down of G
			  if (j == 0)									//top
				  finalBayer(i, j, 0, 2) = bayerFilter(i, j+1, 0, 2);
			  else if (j >= orig.height())					//bottom
				  finalBayer(i, j, 0, 2) = bayerFilter(i, j-1, 0, 2);
			  else {										//middle
				  if (j % 2 == 1) 
					  finalBayer(i, j, 0, 2) = (bayerFilter(i+1,j, 0, 2) + bayerFilter(i-1, j, 0, 2)) / 2;
				  else
					  finalBayer(i, j, 0, 2) = (bayerFilter(i, j + 1, 0, 2) + bayerFilter(i, j - 1, 0, 2)) / 2;
			  }
													// Green values, itself
			  finalBayer(i, j, 0, 1) = bayerFilter(i, j, 0, 1);
		  }

			 // RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR for all red values, add the green values and divide by 2 and same with blue values. 
		  if ((((i % 2) == 1) && ((j % 2) == 0))) {
													// Green values, surrounding R
			  if (i + 1 == orig.width()) {				// right side
				  if (j == 0)								// top right corner
					  finalBayer(i, j, 0, 1) = (bayerFilter(i - 1, j, 0, 1) + bayerFilter(i, j + 1, 0, 1)) / 2;
				  else										// right side middle
					  finalBayer(i, j, 0, 1) = (bayerFilter(i - 1, j, 0, 1) + bayerFilter(i, j - 1, 0, 1) + bayerFilter(i, j + 1, 0, 1)) / 3;
			  }
			  else if (j == 0)							//top row
				  finalBayer(i, j, 0, 1) = (bayerFilter(i + 1, j, 0, 1) + bayerFilter(i - 1, j, 0, 1) + bayerFilter(i, j + 1, 0, 1)) / 3;
			  else
			      finalBayer(i, j, 0, 1) = (bayerFilter(i - 1, j, 0, 1) + bayerFilter(i + 1, j, 0, 1) + bayerFilter(i, j - 1, 0, 1) + bayerFilter(i, j + 1, 0, 1)) / 4;
													// Blue Values, four corners
			  if (i + 1 >= orig.width()) {				//right side
				  if (j == 0)								// top right corner
					  finalBayer(i, j, 0, 2) = bayerFilter(i - 1, j + 1, 0, 2);
				  else										// middle
					  finalBayer(i, j, 0, 2) = (bayerFilter(i - 1, j - 1, 0, 2) + bayerFilter(i - 1, j + 1, 0, 2)) / 2;
			  }
			  else if (j == 0)							// top
				  finalBayer(i, j, 0, 2) = (bayerFilter(i - 1, j + 1, 0, 2) + bayerFilter(i + 1, j + 1, 0, 2)) / 2;
			  else											// middle
				  finalBayer(i, j, 0, 2) = (bayerFilter(i - 1, j - 1, 0, 2) + bayerFilter(i - 1, j + 1, 0, 2) + bayerFilter(i + 1, j - 1, 0, 2) + bayerFilter(i + 1, j + 1, 0, 2)) / 4;
													// Red Values
			  finalBayer(i, j, 0, 0) = bayerFilter(i, j, 0, 0);
		  }
		  
			// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB for all Blue values, add the green values and divide by 2 and same with red values. 
		  if (((i % 2) == 0) && ((j % 2) == 1)) {
													// Green values, surrounding it
			  if (i == 0) {								// left side
				  if (j+1 >= orig.height())					// bottom corner
					  finalBayer(i, j, 0, 1) = (bayerFilter(i + 1, j, 0, 1) + bayerFilter(i, j - 1, 0, 1)) / 2;
				  else										// left side middle
					  finalBayer(i, j, 0, 1) = (bayerFilter(i + 1, j, 0, 1) + bayerFilter(i, j - 1, 0, 1) + bayerFilter(i, j + 1, 0, 1)) / 3;
			  }
			  else if (j+1 >= orig.height())			// bottom
				  finalBayer(i, j, 0, 1) = (bayerFilter(i + 1, j, 0, 1) + bayerFilter(i, j - 1, 0, 1) + bayerFilter(i-1, j, 0, 1)) / 3;
			  else										// middle
				  finalBayer(i, j, 0, 1) = (bayerFilter(i + 1, j, 0, 1) + bayerFilter(i, j - 1, 0, 1) + bayerFilter(i, j + 1, 0, 1) + bayerFilter(i - 1, j, 0, 1)) / 4;
													// Red values, four corners
			  if (i == 0)								// left side
				  if (j + 1 >= orig.height())				// bottom left corner
					  finalBayer(i, j, 0, 0) = bayerFilter(i + 1, j - 1, 0, 0);
				  else										// left side middle
					  finalBayer(i, j, 0, 0) = (bayerFilter(i + 1, j - 1, 0, 0) + bayerFilter(i + 1, j + 1, 0, 0)) / 2;
			  else if (j + 1 >= orig.height())				// bottom edge
				  finalBayer(i, j, 0, 0) = (bayerFilter(i + 1, j - 1, 0, 0) + bayerFilter(i - 1, j - 1, 0, 0)) / 2;
			  else											// middle
				  finalBayer(i, j, 0, 0) = (bayerFilter(i + 1, j + 1, 0, 0) + bayerFilter(i + 1, j - 1, 0, 0) + bayerFilter(i - 1, j + 1, 0, 0) + bayerFilter(i - 1, j - 1, 0, 0)) / 4;
													// Blue values, itself
			  finalBayer(i, j, 0, 2) = bayerFilter(i, j, 0, 2);
		  }
	  }
  }

  //Display and save reconstructed image  
  CImgDisplay disp(finalBayer);
  while (!disp.is_closed())
	  disp.wait();

  finalBayer.save("final.bmp");
  
  return 0;
}
