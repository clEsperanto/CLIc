
#include <random>

#include "cleCustomKernel.hpp"
#include "cleUtils.hpp"
#include "clesperanto.hpp"

int
main (int argc, char **argv)
{
    // Test Initialisation
    using type = float;
    size_t width (10), height (10), depth (10);
    std::array<size_t, 3> shape = { width, height, depth };
    std::vector<type> arr_in (width * height * depth);
    std::vector<type> arr_res (width * height * depth);
    float scalar = 100.0f;
    std::fill (arr_in.begin (), arr_in.end (), static_cast<type> (5));
    std::fill (arr_res.begin (), arr_res.end (), static_cast<type> (5 + scalar));

    const char *name = "add_image_and_scalar";
    std::vector<std::string> tags = { "src", "dst", "scalar" };
    std::string dirpath = argv[1];
    std::string path = dirpath + "/add_image_and_scalar.cl";

    // Test Kernel
    cle::Clesperanto cle;
    auto Buffer_A = cle.Push<type> (arr_in, shape);
    auto Buffer_B = cle.Create<type> (shape);
    cle::CustomKernel kernel (cle.Ressources (), name, tags);
    kernel.SetSource (path.c_str ());
    kernel.SetParameter (Buffer_A, "src");
    kernel.SetParameter (Buffer_B, "dst");
    kernel.SetParameter (scalar, "scalar");
    kernel.Execute ();
    auto arr_out = cle.Pull<type> (Buffer_B);

    // Test Validation
    float difference = 0;
    for (auto it1 = arr_res.begin (), it2 = arr_out.begin ();
         it1 != arr_res.end () && it2 != arr_out.end (); ++it1, ++it2)
        {
            difference += std::abs (*it1 - *it2);
        }
    return difference > std::numeric_limits<type>::epsilon ();
}