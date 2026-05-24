# Dummy

## CONFIG_LEPTO_SIGNAL_NO_METHOD_AS_FUNCTION

Don't use the CFunctorMethodAsFunction class when using signals.
CFunctorMethodAsFunction can avoid some virtual functions but is alittle 
bity hacky. It uses regular function pointers for calling Class-methods.

Enabling this config costs 36 Bytes on miniminutnik.
