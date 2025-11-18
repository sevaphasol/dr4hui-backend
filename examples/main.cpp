// #include <cstdlib>
// #include <dlfcn.h>
// #include <iostream>
// #include <stdexcept>
//
// #include "dr4/event.hpp"
// #include "dr4/math/color.hpp"
// #include "dr4/window.hpp"
//
// using GetBackendT = dr4::DR4Backend* (*)();
//
// int
// main()
// {
//     // std::cout << "Hello world!";
//
//     std::string libdr4Path( dr4::DR4BackendFunctionName );
//
//     void* libdr4 = dlopen( "./install/lib/libdr4.so", RTLD_LAZY );
//
//     if ( !libdr4 )
//     {
//         const char* err = dlerror();
//         std::cerr << "dlopen failed: " << ( err ? err : "unknown error" ) << std::endl;
//         throw std::runtime_error( "Can't open lib" );
//         throw std::runtime_error( "Can't open lib" );
//     }
//
//     dr4::DR4Backend* backend =
//         reinterpret_cast<GetBackendT>( dlsym( libdr4, dr4::DR4BackendFunctionName ) )();
//
//     if ( !backend )
//     {
//         throw std::runtime_error( "Can't get backend" );
//     }
//
//     dr4::Window* window = backend->CreateWindow();
//
//     if ( !window )
//     {
//         throw std::runtime_error( "Can't get window" );
//     }
//
//     window->SetSize( { 500, 500 } );
//     window->SetTitle( "0xCEBAEBA1DEDA" );
//
//     window->Open();
//
//     std::optional<dr4::Event> event = {};
//     while ( window->IsOpen() )
//     {
//
//         while ( ( event = window->PollEvent() ) )
//         {
//             switch ( event->type )
//             {
//                 case dr4::Event::Type::QUIT:
//                     window->Close();
//                     break;
//                 default:
//                     break;
//             }
//         }
//
//         window->Clear( dr4::Color( 100, 100, 100, 255 ) );
//
//         window->Display();
//     }
//
//     delete backend;
//     delete window;
//
//     return EXIT_SUCCESS;
// }
