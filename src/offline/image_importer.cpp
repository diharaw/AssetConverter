#include <offline/image_importer.h>
#include <common/filesystem.h>
#include <thread>
#include <nvtt/nvtt.h>
#include <nvimage/Image.h>
#include <nvimage/DirectDrawSurface.h>
#include <../dependency/stb/stb_image.h>

namespace ast
{
    bool import_image(Image& img, const std::string& file, const PixelType& type, int force_cmp)
    {
        auto ext = filesystem::get_file_extention(file);
        img.name = filesystem::get_filename(file);
        
        if (ext == "dds")
        {
            // Use NVTT for DDS files
            nv::DirectDrawSurface dds;
            
            if (dds.load(file.c_str()))
            {
                nv::Image nv_img;
                dds.mipmap(&nv_img, 0, 0);
                
                img.data[0][0].width = nv_img.width();
                img.data[0][0].height = nv_img.height();
                
                auto fmt = nv_img.format();
                
                if (fmt == nv::Image::Format_RGB)
                    img.components = 3;
                else if (fmt == nv::Image::Format_ARGB)
                    img.components = 4;
                else
                    return false;
                
                size_t size = img.data[0][0].width * img.data[0][0].height * img.components * size_t(type);
                img.data[0][0].data = malloc(size);
                memcpy(img.data[0][0].data, nv_img.pixels(), size);
                
                img.array_slices = 1;
                img.mip_slices = 1;
                
                return true;
            }
        }
        else
        {
            if (type == PIXEL_TYPE_UNORM8)
                img.data[0][0].data = stbi_load(file.c_str(), &img.data[0][0].width, &img.data[0][0].height, &img.components, force_cmp);
            else if (type == PIXEL_TYPE_FLOAT32)
                img.data[0][0].data = stbi_loadf(file.c_str(), &img.data[0][0].width, &img.data[0][0].height, &img.components, force_cmp);
            
            if (img.data[0][0].data != nullptr)
            {
                img.array_slices = 1;
                img.mip_slices = 1;
                
                return true;
            }
        }
        
        return false;
    }
}
