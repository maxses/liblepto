/**
  \page imago Imago

  The class CImago can be used to implement tests for functional sequence over
  time running on host computer.

  Catch2 can be used as framework for testing while CImago will take care about
  feeding the event loop of the embedded framework. CImago also provides
  functions to handle allowed jitter.

  For example an light switch shall fade the brightness of an illumination
  via PWM. When you switch the light on, the light shall fade from 0%
  brightness too 100% brightness in 10 seconds.
  ```
      [...]

      TEST_CASE( "Fader", "[default]" ) {
         /* Pseudo class CFader fading from one PWM value to another in a given time*/
         CFader fader;
         const int FADING_TIME_SEC=10;
         const int FULL_BRIGHTNESS=1000;

         fader.updateTime(0, FULL_BRIGHTNESS, FADING_TIME_SEC);

         auto eventLoop = [ &fader ] ( void )
         {
            biwakEventLoop();
            fader.eventLoop();
            return;
         };

         CImago< typeof( eventLoop) > imago( eventLoop );

         SECTION( "rising" )
         {
            REQUIRE ( fader.current() == 0 );
            fader.setTarget(1000);
            imago.waitTimeMs( ( MSECS_PER_SEC * FADING_TIME_SEC ) / 2 );
            REQUIRE ( imago.approximately(fader.current(), FULL_BRIGHTNESS / 2, 25)
                     == FULL_BRIGHTNESS / 2 );
         }
      }

      [...]
  ```
*/
