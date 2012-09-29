/**
  \page signal Signals

  The class CSignal imlements an signal/slot mechanism. It works both with
  plain functions as well as with class methods.

  Example for using plain function:
   ```
   void functionSlot( int value )
   {
      // Do stuff...
   };

   int main( int argc, const cahr* argv[] )
   {
      CSignal<void, int>sig;

      sig.connect( &functionSlot );
      sig.emitSignal( 0x1234 );
   }
   ```

  Example for using class method:
   ```
   class C1
   {
      public:
         void slot( int add )
         {
            // do stuff
         }
   };

   int main( int argc, const cahr* argv[] )
   {
      C1 obj;
      CSignal<void, int, int>sig;

      sig.connect(&obj, &C1::slot);
      sig.emitSignal( 0x1234 );
   }
   ```
*/
