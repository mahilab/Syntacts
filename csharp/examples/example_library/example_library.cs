using System;
using System.Threading;
using Syntacts;

// run with the following console command:
// dotnet run

class Example
{
    // Show pass/fail for testing purposes (you don't need this)
    static void showResult(bool r) {
        if (r){
            Console.WriteLine("Pass");
        }
        else{
            Console.WriteLine("Fail");
        }
    }
    static void Main(string[] args)
    {
        // Make a Signal to save/export, and a blank Signal to import in to
        Signal save = new Sine(440) * new ASR(1,1,1);
        Signal loaded;

        // Syntacts Binary Format (Default Location, i.e. APPDATA/Syntacts/Library)
        
        showResult( Library.SaveSignal(save, "cs"));
        showResult( Library.LoadSignal(out loaded, "cs"));

        // Syntacts Binary Format (Custom Location)

        showResult( Library.ExportSignal(save, "cs.sig"));
        showResult( Library.ImportSignal(out loaded, "cs.sig"));

        showResult( Library.ExportSignal(save, "relative/folder/cs.sig"));
        showResult( Library.ImportSignal(out loaded, "relative/folder/cs.sig"));

        showResult( Library.ExportSignal(save, "/absolute/folder/cs.sig"));
        showResult( Library.ImportSignal(out loaded, "/absolute/folder/cs.sig"));

        // Syntacts JSON Format

        showResult( Library.ExportSignal(save, "cs.json"));
        showResult( Library.ImportSignal(out loaded, "cs.json"));

        showResult( Library.ExportSignal(save, "relative/folder/cs.json"));
        showResult( Library.ImportSignal(out loaded, "relative/folder/cs.json"));

        showResult( Library.ExportSignal(save, "/absolute/folder/cs.json"));
        showResult( Library.ImportSignal(out loaded, "/absolute/folder/cs.json"));

        // // WAV Format

        showResult( Library.ExportSignal(save, "cs.wav"));
        showResult( Library.ImportSignal(out loaded, "cs.wav"));

        showResult( Library.ExportSignal(save, "relative/folder/cs.wav"));
        showResult( Library.ImportSignal(out loaded, "relative/folder/cs.wav"));

        showResult( Library.ExportSignal(save, "/absolute/folder/cs.wav"));
        showResult( Library.ImportSignal(out loaded, "/absolute/folder/cs.wav"));

        // // AIFF Format

        showResult( Library.ExportSignal(save, "cs.aiff"));
        showResult( Library.ImportSignal(out loaded, "cs.aiff"));

        showResult( Library.ExportSignal(save, "relative/folder/cs.aiff"));
        showResult( Library.ImportSignal(out loaded, "relative/folder/cs.aiff"));

        showResult( Library.ExportSignal(save, "/absolute/folder/cs.aiff"));
        showResult( Library.ImportSignal(out loaded, "/absolute/folder/cs.aiff"));

        // // CSV/TXT Format (import not yet supported)

        showResult( Library.ExportSignal(save, "cs.csv"));
        showResult( Library.ExportSignal(save, "relative/folder/cs.txt"));
        showResult( Library.ExportSignal(save, "/absolute/folder/cs.txt"));

    }
}

