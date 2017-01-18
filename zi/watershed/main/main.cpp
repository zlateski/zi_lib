#include <utility>
#include <zi/arguments.hpp>
#include <zi/watershed/xxl/watershed.hpp>

#include <cstddef>
#include <string>

ZiARG_string( filename,  "./test/t1", "watershed input/output" );
ZiARG_double( high, 0.99, "high thold" );
ZiARG_double( low, 0.1, "low thold" );
ZiARG_int32(  dust, 25, "dust thold" );
ZiARG_double( dust_low, 0.1, "dust low thold" );
ZiARG_int32(  threads, 4, "number of threads for the watershed" );
ZiARG_bool( verbose, false, "show more info during execution" );

struct watershed_task
{
    int32_t affinity_bitsize;
    int32_t id_bitsize      ;
    int32_t x;
    int32_t y;
    int32_t z;
};

int main( int argc, char **argv )
{
    zi::parse_arguments( argc, argv );


    std::string filename = ZiARG_filename;
    std::string f = filename + ".metadata";

    watershed_task ws;
    zi::watershed::mmap_file::read_n( f, &ws, 1 );

    std::size_t total = ws.x * ws.y * ws.z;
    zi::watershed::chunk_dimensions chunk_sizes[ total ];


    f = filename + ".chunksizes";
    zi::watershed::mmap_file::read( f, chunk_sizes, total * 12 );

    if ( ws.affinity_bitsize == 32 )
    {
        if ( ws.id_bitsize == 32 )
        {
            zi::watershed::xxl::watershed_impl< float, uint32_t, uint32_t >
                newws( filename,
                       static_cast< float >( ZiARG_high ),
                       static_cast< float >( ZiARG_low ),
                       static_cast< uint32_t >( ZiARG_dust ),
                       static_cast< float >( ZiARG_dust_low ),
                       ws.x, ws.y, ws.z,
                       chunk_sizes,
                       ZiARG_threads,
                       ZiARG_verbose);

            newws();
        }
        else if ( ws.id_bitsize == 64 )
        {
            zi::watershed::xxl::watershed_impl< float, uint64_t, uint64_t >
                newws( filename,
                       static_cast< float >( ZiARG_high ),
                       static_cast< float >( ZiARG_low ),
                       static_cast< uint64_t >( ZiARG_dust ),
                       static_cast< float >( ZiARG_dust_low ),
                       ws.x, ws.y, ws.z,
                       chunk_sizes,
                       ZiARG_threads,
                       ZiARG_verbose);

            newws();
        }
    }
    else if ( ws.affinity_bitsize == 64 )
    {
        if ( ws.id_bitsize == 32 )
        {
            zi::watershed::xxl::watershed_impl< float, uint32_t, uint32_t >
                newws( filename,
                       static_cast< float >( ZiARG_high ),
                       static_cast< float >( ZiARG_low ),
                       static_cast< uint32_t >( ZiARG_dust ),
                       static_cast< float >( ZiARG_dust_low ),
                       ws.x, ws.y, ws.z,
                       chunk_sizes,
                       ZiARG_threads,
                       ZiARG_verbose);

            newws();
        }
        else if ( ws.id_bitsize == 64 )
        {
            zi::watershed::xxl::watershed_impl< double, uint64_t, uint64_t >
                newws( filename,
                       static_cast< double >( ZiARG_high ),
                       static_cast< double >( ZiARG_low ),
                       static_cast< uint64_t >( ZiARG_dust ),
                       static_cast< double >( ZiARG_dust_low ),
                       ws.x, ws.y, ws.z,
                       chunk_sizes,
                       ZiARG_threads,
                       ZiARG_verbose);

            newws();
        }
    }

    return 0;

}
