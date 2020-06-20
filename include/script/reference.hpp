#pragma once

namespace Script
{
     class iReference
     {
     public: 
          iReference();
          void AddRef();
          void Release();
     private:
          int ref_count;
     };
};