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
        
        showResult( Library.SaveSignal(save, "saved"));
        showResult( Library.LoadSignal(out loaded, "saved"));

        // Syntacts Binary Format (Custom Location)

        showResult( Library.ExportSignal(save, "saved.sig"));
        showResult( Library.ImportSignal(out loaded, "saved.sig"));

        showResult( Library.ExportSignal(save, "relative/folder/saved.sig"));
        showResult( Library.ImportSignal(out loaded, "relative/folder/saved.sig"));

        showResult( Library.ExportSignal(save, "/absolute/folder/saved.sig"));
        showResult( Library.ImportSignal(out loaded, "/absolute/folder/saved.sig"));

        // Syntacts JSON Format

        showResult( Library.ExportSignal(save, "saved.json"));
        showResult( Library.ImportSignal(out loaded, "saved.json"));

        showResult( Library.ExportSignal(save, "relative/folder/saved.json"));
        showResult( Library.ImportSignal(out loaded, "relative/folder/saved.json"));

        showResult( Library.ExportSignal(save, "/absolute/folder/saved.json"));
        showResult( Library.ImportSignal(out loaded, "/absolute/folder/saved.json"));

        // // WAV Format

        showResult( Library.ExportSignal(save, "saved.wav"));
        showResult( Library.ImportSignal(out loaded, "saved.wav"));

        showResult( Library.ExportSignal(save, "relative/folder/saved.wav"));
        showResult( Library.ImportSignal(out loaded, "relative/folder/saved.wav"));

        showResult( Library.ExportSignal(save, "/absolute/folder/saved.wav"));
        showResult( Library.ImportSignal(out loaded, "/absolute/folder/saved.wav"));

        // // AIFF Format

        showResult( Library.ExportSignal(save, "saved.aiff"));
        showResult( Library.ImportSignal(out loaded, "saved.aiff"));

        showResult( Library.ExportSignal(save, "relative/folder/saved.aiff"));
        showResult( Library.ImportSignal(out loaded, "relative/folder/saved.aiff"));

        showResult( Library.ExportSignal(save, "/absolute/folder/saved.aiff"));
        showResult( Library.ImportSignal(out loaded, "/absolute/folder/saved.aiff"));

        // // CSV/TXT Format (import not yet supported)

        showResult( Library.ExportSignal(save, "saved.txt"));
        showResult( Library.ExportSignal(save, "relative/folder/saved.txt"));
        showResult( Library.ExportSignal(save, "/absolute/folder/saved.txt"));

    }
}

