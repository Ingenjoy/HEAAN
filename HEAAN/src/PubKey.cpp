#include "PubKey.h"

#include "NumUtils.h"
#include "Params.h"
#include "Ring2Utils.h"

PubKey::PubKey(Params& params, SecKey& secretKey) {
	ZZX ex, sxsx;

	NumUtils::sampleUniform2(ax, params.N, params.logq);
	NumUtils::sampleGauss(ex, params.N, params.sigma);
	Ring2Utils::mult(bx, secretKey.sx, ax, params.q, params.N);
	Ring2Utils::sub(bx, ex, bx, params.q, params.N);

	//-----------------------------------------

	axLeftRot = new ZZX[params.logN - 1];
	bxLeftRot = new ZZX[params.logN - 1];

	for (long i = 0; i < (params.logN - 1); ++i) {
		ZZX spow;
		long ipow = (1 << i);
		Ring2Utils::inpower(spow, secretKey.sx, params.rotGroup[params.logN - 1][ipow], params.q, params.N);
		Ring2Utils::leftShiftAndEqual(spow, params.logP, params.Pq, params.N);
		NumUtils::sampleUniform2(axLeftRot[i], params.N, params.logPq);
		NumUtils::sampleGauss(ex, params.N, params.sigma);
		Ring2Utils::addAndEqual(ex, spow, params.Pq, params.N);
		Ring2Utils::mult(bxLeftRot[i], secretKey.sx, axLeftRot[i], params.Pq, params.N);
		Ring2Utils::sub(bxLeftRot[i], ex, bxLeftRot[i], params.Pq, params.N);
	}

	axRightRot = new ZZX[params.logN - 1];
	bxRightRot = new ZZX[params.logN - 1];

	for (long i = 0; i < (params.logN - 1); ++i) {
		ZZX spow;
		long ipow = params.N/2 - (1 << i);
		Ring2Utils::inpower(spow, secretKey.sx, params.rotGroup[params.logN - 1][ipow], params.q, params.N);
		Ring2Utils::leftShiftAndEqual(spow, params.logP, params.Pq, params.N);
		NumUtils::sampleUniform2(axRightRot[i], params.N, params.logPq);
		NumUtils::sampleGauss(ex, params.N, params.sigma);
		Ring2Utils::addAndEqual(ex, spow, params.Pq, params.N);
		Ring2Utils::mult(bxRightRot[i], secretKey.sx, axRightRot[i], params.Pq, params.N);
		Ring2Utils::sub(bxRightRot[i], ex, bxRightRot[i], params.Pq, params.N);
	}

	//-----------------------------------------

	Ring2Utils::mult(sxsx, secretKey.sx, secretKey.sx, params.q, params.N);
	Ring2Utils::leftShiftAndEqual(sxsx, params.logP, params.Pq, params.N);
	NumUtils::sampleUniform2(axStar, params.N, params.logPq);
	NumUtils::sampleGauss(ex, params.N, params.sigma);
	Ring2Utils::addAndEqual(ex, sxsx, params.Pq, params.N);
	Ring2Utils::mult(bxStar, secretKey.sx, axStar, params.Pq, params.N);
	Ring2Utils::sub(bxStar, ex, bxStar, params.Pq, params.N);

	ZZX sxconj;
	Ring2Utils::conjugate(sxconj, secretKey.sx, params.N);
	Ring2Utils::leftShiftAndEqual(sxconj, params.logP, params.Pq, params.N);
	NumUtils::sampleUniform2(axConj, params.N, params.logPq);
	NumUtils::sampleGauss(ex, params.N, params.sigma);
	Ring2Utils::addAndEqual(ex, sxconj, params.Pq, params.N);
	Ring2Utils::mult(bxConj, secretKey.sx, axConj, params.Pq, params.N);
	Ring2Utils::sub(bxConj, ex, bxConj, params.Pq, params.N);

	//-----------------------------------------
}
