
#include "readIMG.h"
#include <stdio.h>

int main(int argc, char** argv) {

	HFAImage i("d:\\working\\image.img");
//	i.printDictionaryString();
	i.displayDictionary();
	return(0);
}