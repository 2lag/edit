#include "config.h"

bool menu_style_toggle[ OBJ_BASE_COUNT / 2 ];

void load_config() {
  char *doc_path = nullptr;
  size_t doc_len = 0;
  _dupenv_s( &doc_path, &doc_len, "USERPROFILE" );

  if( !doc_path ) {
#ifdef _DEBUG
    printf("failed @ docpath\n");
#endif
    return;
  }

  doc_len += strlen("\\Documents\\edit\\edit.cfg") + 1;
  char *cfg_path = new char[ doc_len ];
  strcpy_s( cfg_path, doc_len, doc_path );
  strcat_s( cfg_path, doc_len, "\\Documents\\edit\\edit.cfg" );

  if( !cfg_path ) {
    free( doc_path );
    delete[] cfg_path;
#ifdef _DEBUG
    printf("failed @ cfg path\n");
#endif
    return;
  }
  
  HANDLE cfg_file = CreateFileA( cfg_path,
    GENERIC_READ, 0, NULL, OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL, NULL
  );

  if( cfg_file == INVALID_HANDLE_VALUE || GetLastError() == ERROR_FILE_NOT_FOUND ) {
    free( doc_path );
    delete[] cfg_path;
#ifdef _DEBUG
    if( cfg_file == INVALID_HANDLE_VALUE )
      printf("failed @ invalid_handle_value : %d\n", GetLastError() );
    else
      printf("failed @ cfg file not found : %d\n", GetLastError() );
#endif
    return;
  }

  u64 cfg;
  ptr bytes_read;
  if( !ReadFile( cfg_file, &cfg, 1, &bytes_read, NULL ) ) {
    free( doc_path );
    delete[] cfg_path;
    CloseHandle( cfg_file );
  #ifdef _DEBUG
    printf("failed @ reading cfg file\n");
  #endif
    return;
  }

  for( s32 i = 0; i < 3; i++ )
    printf( "%llu", ( cfg >> i ) & 1 );

  free( doc_path );
  delete[] cfg_path;
  CloseHandle( cfg_file );
  return;
}

void init_config() {
  for( s32 i = 0; i < 3; i++ )
      menu_style_toggle[ i ] = true;

  char *doc_path = nullptr;
  size_t doc_len = 0;
  _dupenv_s( &doc_path, &doc_len, "USERPROFILE" );

  if( !doc_path ) {
#ifdef _DEBUG
    printf("failed @ docpath\n");
#endif
    return;
  }

  size_t edit_path_len = doc_len + strlen("\\Documents\\edit\\") + 1;
  char *edit_path = new char[ edit_path_len ];
  strcpy_s( edit_path, edit_path_len, doc_path );
  strcat_s( edit_path, edit_path_len, "\\Documents\\edit\\" );

  if( !CreateDirectoryA( edit_path, NULL ) ) {
    if( GetLastError() != ERROR_ALREADY_EXISTS ) {
      free( doc_path );
      delete[] edit_path;
#ifdef _DEBUG
      printf("failed @ directory\n");
#endif
      return;
    }
  }

  edit_path_len += strlen("edit.cfg");
  char *cfg_path = new char[ edit_path_len ];
  strcpy_s( cfg_path, edit_path_len, edit_path );
  strcat_s( cfg_path, edit_path_len, "edit.cfg" );

  if( !cfg_path ) {
    free( doc_path );
    delete[] cfg_path;
    delete[] edit_path;
#ifdef _DEBUG
      printf("failed @ cfg path\n");
#endif
    return ;
  }

  HANDLE cfg_file = CreateFileA( cfg_path,
    GENERIC_READ | GENERIC_WRITE,
    FILE_SHARE_READ | FILE_SHARE_WRITE,
    NULL, OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL, NULL
  );

  if( GetLastError() == ERROR_ALREADY_EXISTS ) {
#ifdef _DEBUG
      printf("loading saved config\n");
#endif
    CloseHandle( cfg_file );
    load_config();
  } else {
#ifdef _DEBUG
      printf( "writing config to %s\n", cfg_path );
#endif
    u64 default_cfg = 0b00000111;
    ptr bytes_written;
    WriteFile( cfg_file, &default_cfg, 1, &bytes_written, NULL );
    CloseHandle( cfg_file );
  }

  free( doc_path );
  delete[] cfg_path;
  delete[] edit_path;
  return;
}

bool toggle_config_idx( s32 idx ) {
  // open file ( binary mode? other method? test and find out! )
  // read idx bit
  // toggle idx bit
  // set menu_style_toggle
  // save file
  // close file

  /*
  char *doc_path = nullptr;
  size_t doc_len = 0;
  _dupenv_s(&doc_path, &doc_len, "USERPROFILE");

  if (!doc_path) {
    #ifdef _DEBUG
      printf("failed @ docpath\n");
    #endif
    return false;
  }

  doc_len += strlen("\\Documents\\edit\\edit.cfg") + 1;
  char *cfg_path = new char[doc_len];
  strcpy_s(cfg_path, doc_len, doc_path);
  strcat_s(cfg_path, doc_len, "\\Documents\\edit\\edit.cfg");

  HANDLE cfg_file = CreateFileA(cfg_path,
    GENERIC_READ | GENERIC_WRITE, 0, NULL,
    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
  );

  if (cfg_file == INVALID_HANDLE_VALUE) {
    free(doc_path);
    delete[] cfg_path;
    #ifdef _DEBUG
      printf("failed to open cfg file for toggling\n");
    #endif
    return false;
  }

  u64 cfg;
  ptr bytes_read;
  if (!ReadFile(cfg_file, &cfg, sizeof(cfg), &bytes_read, NULL) || bytes_read != sizeof(cfg)) {
    CloseHandle(cfg_file);
    free(doc_path);
    delete[] cfg_path;
    #ifdef _DEBUG
      printf("failed @ reading cfg for toggle\n");
    #endif
    return false;
  }

  // toggle the bit
  cfg ^= 1ULL << idx;
  menu_style_toggle[idx] = !menu_style_toggle[idx];

  // move the cursor back to the beginning of the file to write
  SetFilePointer(cfg_file, 0, NULL, FILE_BEGIN);
  ptr bytes_written;
  if (!WriteFile(cfg_file, &cfg, sizeof(cfg), &bytes_written, NULL) || bytes_written != sizeof(cfg)) {
    CloseHandle(cfg_file);
    free(doc_path);
    delete[] cfg_path;
    #ifdef _DEBUG
      printf("failed @ writing cfg for toggle\n");
    #endif
    return false;
  }

  CloseHandle(cfg_file);
  free(doc_path);
  delete[] cfg_path;
  return true;
  */
  
  return true;
}
