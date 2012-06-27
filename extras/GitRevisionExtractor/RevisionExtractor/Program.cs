using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace RevisionExtractor
{
    class Program
    {
        static void Main(string[] args)
        {
            Uri pfad = null;
            if (args.Length > 0)
            {
                // süß, ein Pfad oder ? :P
                Uri.TryCreate(args[0] + (args[0][args[0].Length - 1] != '/' ? "/" : ""), UriKind.Absolute, out pfad);
            }

            string user = SystemInformation.UserName;
            string host = SystemInformation.ComputerName;

            if (!File.Exists(pfad == null ? "" : pfad.LocalPath + ".git/FETCH_HEAD"))
            {
                Console.WriteLine("Search for git folder...");
                Uri.TryCreate(FindGitDir(), UriKind.Absolute, out pfad);

                if (pfad == null)
                {
                    Console.WriteLine("Path is invaild or this folder isn't under git version control!\n Exit...");
                    return;
                }
            }

            string[] fileInput = File.ReadAllText(pfad == null ? "" : pfad.LocalPath + ".git/FETCH_HEAD").Split(new string[] { "\t\t" }, StringSplitOptions.None);
            string verHash = fileInput[0];

            Console.WriteLine("Current git revision is {0}", verHash);

            bool gen = true;
            if (File.Exists("git_revision.h"))
            {
                // Ist die vorhandene Datei aktuell...?
                string[] revInput = File.ReadAllLines("git_revision.h");
                if (revInput == null ||
                    revInput.Length != 10 ||
                    revInput[3] != String.Format("#define BUILD_REVISION \"{0}\"", verHash) ||
                    revInput[4] != String.Format("#define BUILD_HOST \"{0}\"", host) ||
                    revInput[5] != String.Format("#define BUILD_USER \"{0}\"", user))
                    File.Delete("git_revision.h");
                else
                    gen = false;
            }

            if (gen == true)
            {
                Console.Write("Generate git_revision.h!");

                FileStream nStream = null;
                try
                {
                    using (nStream = new FileStream(@"git_revision.h", FileMode.OpenOrCreate, FileAccess.Write))
                    {
                        StreamWriter wStream = null;
                        try
                        {
                            using (wStream = new StreamWriter(nStream))
                            {
                                wStream.WriteLine("#ifndef GIT_REVISION_H");
                                wStream.WriteLine("#define GIT_REVISION_H\n");
                                wStream.WriteLine("#define BUILD_REVISION \"{0}\"", verHash);
                                wStream.WriteLine("#define BUILD_HOST \"{0}\"", host);
                                wStream.WriteLine("#define BUILD_USER \"{0}\"", user);
                                wStream.WriteLine("#define BUILD_TIME __TIME__");
                                wStream.WriteLine("#define BUILD_DATE __DATE__\n");
                                wStream.WriteLine("#endif\t\t // GIT_REVISION_H");
                            }
                        }
                        finally
                        {
                            if (wStream != null)
                                wStream.Close();
                        }
                    }
                }
                finally
                {
                    if (nStream != null)
                        nStream.Close();

                    Console.Write("Finished");
                }
            }

            Console.WriteLine("GitExtractor finished!");
        }

        private static string FindGitDir()
        {
            string dir = Environment.CurrentDirectory;
            while (dir.IndexOf('\\') != -1)
            {
                if (File.Exists(String.Format("{0}/.git/FETCH_HEAD", dir)))
                    return dir + "\\";

                int pos = dir.LastIndexOf('\\');
                if (pos == -1)
                    return "";
                dir = dir.Remove(pos);
            }

            return "";
        }
    }
}
