#ifndef SLAWRITER_HPP
#define SLAWRITER_HPP

// For png export of the sliced model
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <array>

#include "libslic3r/PrintConfig.hpp"

#include "SLARaster.hpp"

namespace Slic3r { namespace sla {

// An abstract parent class to bridge different SLA technologies and file formats
// This class will serve as an API for all SLA printers to the slicer
class Writer
{
public:

    // Used for addressing parameters of set_statistics()
    struct PrintStatistics
    {
        double used_material = 0.;
        double estimated_print_time_s = 0.;
        size_t num_fade = 0;
        size_t num_slow = 0;
        size_t num_fast = 0;
    };

private:
    //Technology
    //output types
public:

    // SLARasterWriter is using Raster in custom mirroring mode
    RasterWriter(const Raster::Resolution &res,
                 const Raster::PixelDim &  pixdim,
                 const Raster::Trafo &     trafo,
                 double                    gamma = 1.);

    RasterWriter(const RasterWriter& ) = delete;
    RasterWriter& operator=(const RasterWriter&) = delete;
    RasterWriter(RasterWriter&& m) = default;
    RasterWriter& operator=(RasterWriter&&) = default;

    inline void layers(unsigned cnt) { if(cnt > 0) m_layers_rst.resize(cnt); }
    inline unsigned layers() const { return unsigned(m_layers_rst.size()); }

    template<class Poly> void draw_polygon(const Poly& p, unsigned lyr)
    {
        assert(lyr < m_layers_rst.size());
        m_layers_rst[lyr].raster.draw(p);
    }

    inline void begin_layer(unsigned lyr) {
        if(m_layers_rst.size() <= lyr) m_layers_rst.resize(lyr+1);
        m_layers_rst[lyr].raster.reset(m_res, m_pxdim, m_trafo);
    }

    inline void begin_layer() {
        m_layers_rst.emplace_back();
        m_layers_rst.front().raster.reset(m_res, m_pxdim, m_trafo);
    }

    inline void finish_layer(unsigned lyr_id) {
        assert(lyr_id < m_layers_rst.size());
        m_layers_rst[lyr_id].rawbytes.serialize(m_layers_rst[lyr_id].raster);
        m_layers_rst[lyr_id].raster.reset();
    }

    inline void finish_layer() {
        if(!m_layers_rst.empty()) {
            m_layers_rst.back().rawbytes.serialize(m_layers_rst.back().raster);
            m_layers_rst.back().raster.reset();
        }
    }

    virtual void save(const std::string &fpath, const std::string &prjname = "") = 0;

    void set_statistics(const PrintStatistics &statistics);

    void set_config(const DynamicPrintConfig &cfg);
};

} // namespace sla
} // namespace Slic3r

#endif // SLARASTERWRITER_HPP
