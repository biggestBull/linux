#ifndef DS_TOOLS
#define DS_TOOLS

static inline void
To_char(void * source,void * dest){
   *((char *)dest)=*((char *)source);
}

static inline void
To_int(void * source,void * dest){
   *((int *)dest)=*((int *)source);
}

static inline void
To_long(void * source,void * dest){
   *((long *)dest)=*((long *)source);
           
}
static inline void
To_ptr(void * source,void * dest){
   *((unsigned long *)dest)=*((unsigned long *)source);
}
           
#endif
