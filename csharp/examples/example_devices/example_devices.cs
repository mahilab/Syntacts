using System;
using System.Threading;
using Syntacts;

// run with the following console command:
// dotnet run

class Example
{
    static void Main(string[] args)
    {
        Session session = new Session();
        
        foreach (Device dev in session.availableDevices) {
            Console.WriteLine("");
            Console.WriteLine("Index:        {0}", dev.index);
            Console.WriteLine("Name:         {0}", dev.name); 
            Console.WriteLine("Default:      {0}", dev.isDefault); 
            Console.WriteLine("API:          {0}", dev.apiName); 
            Console.WriteLine("API Default:  {0}", dev.isApiDefault); 
            Console.WriteLine("Max Channels: {0}", dev.maxChannels); 
            Console.WriteLine("Sample Rates: [{0}]", string.Join(", ", dev.sampleRates));
        }
    }

}

