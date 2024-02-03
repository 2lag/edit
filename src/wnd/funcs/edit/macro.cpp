#include "macro.h"

std::vector< std::pair< u8, u16 > > macro; // ew for every time i have to see this
void record_macro( u8 key, u16 status ) {
  u16 status_fix = status;

  if( status == WM_KEYUP )
    status_fix = KEYEVENTF_KEYUP;
  else if( status == WM_KEYDOWN )
    status_fix = 0;
  else
    return;

  if( macro_recording ) {
    macro.push_back( {
      key,
      status_fix
    });
  }
}

void reset_all_keystates() {
  INPUT *inputs = new INPUT[ 256 ];

  for( u8 idx = 0; idx < sizeof( *inputs ); ++idx ) {
    inputs[ idx ].type = INPUT_KEYBOARD;
    ZeroMemory( &inputs[ idx ].ki, sizeof( KEYBDINPUT ) );
    inputs[ idx ].ki.wVk = idx;
    inputs[ idx ].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[ idx ].ki.time = idx;

#ifdef _DEBUG
    printf( "%u\n", idx );
#endif
  }

  u32 sent = SendInput( 256, inputs, sizeof( INPUT ) );

  delete[] inputs;
  
#ifdef _DEBUG
  if( sent != 256 ) {
     printf( "reset sendinput failed : 0x%x\n", HRESULT_FROM_WIN32( GetLastError() ) );
  } else {
     printf( "reset '%u' keys\n", sent );
  }
#endif
}

s32 playback_macro() {
  if( macro.empty() )
    return 1;
  
  reset_all_keystates();

  INPUT *inputs = new INPUT[ macro.size() ];
  memset( inputs, 0,
    sizeof( INPUT ) * macro.size()
  );

  for( u64 idx = 0; idx < macro.size(); idx++ ) {
    auto [ v_key, status ] = macro.at( idx );

    inputs[ idx ].type = INPUT_KEYBOARD;
    inputs[ idx ].ki.wVk = v_key;
    inputs[ idx ].ki.dwFlags = status;

#ifdef _DEBUG
    printf( "\n0x%x\t%s\n",
      v_key,
      !status ? "up" : "down"
    );
#endif
  }

  u32 sent = SendInput(
    static_cast<u32>( macro.size() ),
    inputs,
    sizeof( INPUT )
  );
  
#ifdef _DEBUG
  if( sent != macro.size() ) {
    printf( "sendinput failed : 0x%x\n", HRESULT_FROM_WIN32( GetLastError() ) );
  } else {
    printf( "sendinput sent '%u' inputs", sent );
  }
#endif
  
  delete[] inputs;
  return 1;
}