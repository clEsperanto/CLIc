/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleAbsoluteKernel.h"
#include "cleAddImageAndScalarKernel.h"
#include "cleAddImagesWeightedKernel.h"
#include "cleDilateSphereKernel.h"
#include "cleCopyKernel.h"
#include "cleErodeSphereKernel.h"
#include "cleGaussianBlurKernel.h"
#include "cleGreaterConstantKernel.h"
#include "cleGreaterOrEqualConstantKernel.h"
#include "cleMaximumOfAllPixelsKernel.h"
#include "cleMaximumZProjectionKernel.h"
#include "cleMaximumYProjectionKernel.h"
#include "cleMaximumXProjectionKernel.h"
#include "cleMinimumOfAllPixelsKernel.h"
#include "cleMinimumZProjectionKernel.h"
#include "cleMinimumYProjectionKernel.h"
#include "cleMinimumXProjectionKernel.h"
#include "cleMean2DSphereKernel.h"
#include "cleNonzeroMinimumBoxKernel.h"
#include "cleSmallerOrEqualConstantKernel.h"
#include "cleSobelKernel.h"
#include "cleSetKernel.h"
#include "cleSetNonzeroPixelsToPixelindexKernel.h"

