///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2016 DreamWorks Animation LLC. 
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// *       Redistributions of source code must retain the above
//         copyright notice, this list of conditions and the following
//         disclaimer.
// *       Redistributions in binary form must reproduce the above
//         copyright notice, this list of conditions and the following
//         disclaimer in the documentation and/or other materials
//         provided with the distribution.
// *       Neither the name of DreamWorks Animation nor the names of its
//         contributors may be used to endorse or promote products
//         derived from this software without specific prior written
//         permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////
///
/// @file DcxPixelTile.cpp


#include "DcxPixelTile.h"


OPENDCX_INTERNAL_NAMESPACE_HEADER_ENTER


PixelTile::PixelTile (ChannelContext& channel_ctx,
                      bool yAxisUp) :
    m_yaxis_up(yAxisUp),
    m_top_reference(0),
    m_dataWindow(IMATH_NAMESPACE::V2i(0,0), IMATH_NAMESPACE::V2i(-1,-1)),
    m_channel_ctx(&channel_ctx)
{
    //
}

PixelTile::PixelTile (const OPENEXR_IMF_NAMESPACE::Header& header,
                      ChannelContext& channel_ctx,
                      bool yAxisUp) :
    m_yaxis_up(yAxisUp),
    m_top_reference(header.displayWindow().max.y),
    m_channel_ctx(&channel_ctx)
{
    setDataWindow(header.dataWindow(), true/*sourceWindowsYAxisUp*/); // May flip the data window
}

PixelTile::PixelTile (const IMATH_NAMESPACE::Box2i& display_window,
                      const IMATH_NAMESPACE::Box2i& data_window,
                      bool sourceWindowsYAxisUp,
                      const ChannelSet& channels,
                      ChannelContext& channel_ctx,
                      bool yAxisUp) :
    m_yaxis_up(yAxisUp),
    m_top_reference(display_window.max.y),
    m_dataWindow(data_window),
    m_channel_ctx(&channel_ctx)
{
    setDataWindow(data_window, sourceWindowsYAxisUp); // May flip the data window
    setChannels(channels);
}

PixelTile::PixelTile (const IMATH_NAMESPACE::Box2i& data_window,
                      int top_reference,
                      bool sourceWindowsYAxisUp,
                      const ChannelSet& channels,
                      ChannelContext&  channel_ctx,
                      bool yAxisUp) :
    m_yaxis_up(yAxisUp),
    m_top_reference(top_reference),
    m_dataWindow(data_window),
    m_channel_ctx(&channel_ctx)
{
    setDataWindow(data_window, sourceWindowsYAxisUp); // May flip the data window
    setChannels(channels);
}

PixelTile::PixelTile (const PixelTile& b) :
    m_yaxis_up(b.m_yaxis_up),
    m_top_reference(b.m_top_reference),
    m_dataWindow(b.m_dataWindow),
    m_channel_ctx(b.m_channel_ctx),
    //
    m_channels(b.m_channels)
{
    //
}


/*virtual*/
void
PixelTile::setDataWindow (const IMATH_NAMESPACE::Box2i& data_window,
                          bool sourceWindowYAxisUp)
{
    m_dataWindow = data_window;
    // Flip data window vertically if source window is flipped:
    if (m_yaxis_up != sourceWindowYAxisUp)
    {
        m_dataWindow.min.y = m_top_reference - data_window.max.y;
        m_dataWindow.max.y = m_top_reference - data_window.min.y;
    }
}


/*virtual*/
void
PixelTile::setChannels (const ChannelSet& channels)
{
    m_channels.clear();
    if (!m_channel_ctx)
        return;
    // Verify each channel is valid in this context:
    foreach_channel(z, channels)
    {
        if (m_channel_ctx->findChannelAlias(z))
            m_channels += z;
    }
}


/*virtual*/
PixelTile::~PixelTile ()
{
   //
}


OPENDCX_INTERNAL_NAMESPACE_HEADER_EXIT
