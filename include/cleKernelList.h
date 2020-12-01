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
#include "cleEqualKernel.h"
#include "cleEqualConstantKernel.h"
#include "cleGreaterKernel.h"
#include "cleGreaterOrEqualKernel.h"
#include "cleGreaterConstantKernel.h"
#include "cleGreaterOrEqualConstantKernel.h"
#include "cleMaximumKernel.h"
#include "cleMaximumOfAllPixelsKernel.h"
#include "cleMaximumZProjectionKernel.h"
#include "cleMaximumYProjectionKernel.h"
#include "cleMaximumXProjectionKernel.h"
#include "cleMeanKernel.h"
#include "cleMinimumKernel.h"
#include "cleMinimumOfAllPixelsKernel.h"
#include "cleMinimumZProjectionKernel.h"
#include "cleMinimumYProjectionKernel.h"
#include "cleMinimumXProjectionKernel.h"
#include "cleMean2DSphereKernel.h"
#include "cleNonzeroMinimumBoxKernel.h"
#include "cleNotEqualKernel.h"
#include "cleNotEqualConstantKernel.h"
#include "cleSmallerKernel.h"
#include "cleSmallerOrEqualKernel.h"
#include "cleSmallerConstantKernel.h"
#include "cleSmallerOrEqualConstantKernel.h"
#include "cleSobelKernel.h"
#include "cleSetKernel.h"
#include "cleSetNonzeroPixelsToPixelindexKernel.h"
#include "cleDifferenceOfGaussianKernel.h"
#include "cleDetectMaximaKernel.h"
#include "cleSumOfAllPixelsKernel.h"
#include "cleSumZProjectionKernel.h"
#include "cleSumYProjectionKernel.h"
#include "cleSumXProjectionKernel.h"
