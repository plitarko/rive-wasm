/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrRectanizerSkyline_DEFINED
#define GrRectanizerSkyline_DEFINED

#include "include/private/SkTDArray.h"
#include "src/gpu/GrRectanizer.h"

// Pack rectangles and track the current silhouette
// Based, in part, on Jukka Jylanki's work at http://clb.demon.fi
//
// Mark this class final in an effort to avoid the vtable when this subclass is used explicitly.
class GrRectanizerSkyline final : public GrRectanizer
{
public:
    GrRectanizerSkyline(int w, int h) : INHERITED(w, h) { this->reset(); }

    ~GrRectanizerSkyline() final {}

    void reset() final
    {
        fAreaSoFar = 0;
        fSkyline.reset();
        SkylineSegment* seg = fSkyline.append(1);
        seg->fX = 0;
        seg->fY = 0;
        seg->fWidth = this->width();
    }

    bool addRect(int w, int h, SkIPoint16* loc) final;

    float percentFull() const final { return fAreaSoFar / ((float)this->width() * this->height()); }

private:
    struct SkylineSegment
    {
        int fX;
        int fY;
        int fWidth;
    };

    SkTDArray<SkylineSegment> fSkyline;

    int32_t fAreaSoFar;

    // Can a width x height rectangle fit in the free space represented by
    // the skyline segments >= 'skylineIndex'? If so, return true and fill in
    // 'y' with the y-location at which it fits (the x location is pulled from
    // 'skylineIndex's segment.
    bool rectangleFits(int skylineIndex, int width, int height, int* y) const;
    // Update the skyline structure to include a width x height rect located
    // at x,y.
    void addSkylineLevel(int skylineIndex, int x, int y, int width, int height);

    using INHERITED = GrRectanizer;
};

#endif
