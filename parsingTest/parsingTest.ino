#include "ParseUtil.h"

void loop()
{   }


void setup()
{
    Serial.begin(9600);
    delay(5000);

    String fluffy = "XPLORE,4.52,5.21,34,GOTO,9.2,8.7,4,GOTO,103.2,421.7,450";
    
    const char*  str_data       = fluffy.c_str();//"XPLORE,4.52,5.21,34,GOTO,9.2,8.7,4";
    size_t  num_parameters      = 0;
    size_t  len_parameter       = 0;
    
    param_count_and_length(str_data, num_parameters, len_parameter);

    const size_t PARAMS_MAX    = num_parameters;
    const size_t LENGTH_MAX    = len_parameter;
    char szParams[PARAMS_MAX][LENGTH_MAX + 1];

    char* pnext = (char*)extract_token(&szParams[0][0], str_data);
    Serial.print("szParams[0]: ");
    Serial.println(szParams[0]);

    for ( size_t i = 1; i < num_parameters; i++ )
    {
        pnext = (char*)extract_token(&szParams[i][0], pnext);
        Serial.print("szParams[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(szParams[i]);
    }
}
