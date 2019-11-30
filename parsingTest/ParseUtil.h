
void param_count_and_length(const char*  str_data, size_t& num_params, size_t& length)
{
    size_t  t_max     = 0;
    const char* src   = str_data;
    const char* p = nullptr;
    char    ch;
    while ( (ch = *src++) )
    {
        t_max++;
        
        if ( ',' == ch )
        {
            p = src;
            
            num_params++;
            if ( t_max > length )
            {
                length = t_max;
                t_max  = 0;
            }
        }
    }
    
    const size_t  count_remaining = strlen(p);
    if ( count_remaining )
    {
        num_params++;
        
        if ( count_remaining > length )
        {
            length = count_remaining;
        }
    }
}

const char* extract_token(char* dst, const char* src, char const sep = ',')
{
    while ( *src )
    {
        if ( sep == *src )
        {
            *dst = '\0';
            
            return ++src;
        }
        
        *dst++ = *src++;
    }
    
    *dst = '\0';
    
    return NULL;
}
