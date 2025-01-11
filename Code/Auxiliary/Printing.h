//
// Created by Guglielmo Mazzesi on 12/1/2024.
//

#ifndef PRINTING_H
#define PRINTING_H

inline void PrintExecutionTime (const clock_t time,  const string & message) {
    // Printing the provided time and message
    cout << fixed << setprecision(2) << "It took " << ((float)time)/CLOCKS_PER_SEC<< " seconds (" <<
                    (float)time / CLOCKS_PER_SEC / 60 << " minutes) to " << message << endl << endl;
}

inline void PrintGenericMessage(const string & message) {
    cout << fixed << setprecision(2) << message << endl;
}

inline void PrintStartingProcess(const string & message) {
    cout << "Starting " << message << "..." << endl;
}

inline void PrintError (const string & message) {
    cerr << message << endl;
}

inline void PrintVec3 (const string & introduction, const glm::vec3 & vector) {
    cout << introduction << vector.x << ", " << vector.y << ", " << vector.z << endl;
}

#endif //PRINTING_H
