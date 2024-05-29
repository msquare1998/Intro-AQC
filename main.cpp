/* ================================================
 *  AQC for 2-SAT problem
 *  Author: Yiming Ding, Westlake Univ
 *  Last updated: Jan.9, 2024
 * ================================================*/
#include <iostream>
#include "QuEST.h"
#define a 0     // label of the ancilla
using namespace std;

int main (){
    int pair[][ 2 ] = {
            { 1, 2 },
            { 2, 3 },
            { 1, 3 }
    };

    /* ====================================
     * Environment initialization
     * ===================================*/
    int nQ = 4;         // number of qubits
    QuESTEnv env = createQuESTEnv();
    Qureg circ = createQureg( nQ, env );
    Qureg workspace = createQureg( nQ, env );
    initZeroState( circ );

    /* ====================================
     *  Prepare the initial state
     * ===================================*/
    for ( int i = 0; i < 4; ++i ) {
        pauliX( circ, i );
        hadamard( circ, i );
    }

    /* ====================================
     *  Adiabatic evolution
     * ===================================*/
    double dt = 0.05;    // evolution time in each step
    double step_size = 0.0005;      // variation of s in each step
    auto N = int( 1 / step_size );
    double s = 0.0;

    for ( int n = 0; n < N; ++n ) {
        s += step_size;
        // H_1
        for ( int i = 1; i < 4; ++i )
            rotateX( circ, i, 2 * ( 1 - s ) * dt );
        // H_{f_1}
        for ( int j = 0; j < 2; ++j )
            controlledNot( circ, pair[ 0 ][ j ], a );
        rotateZ( circ, a, -2 * s * dt );
        for ( int j = 1; j > -1; --j )
            controlledNot( circ, pair[ 0 ][ j ], a );
        // H_{f_2} and H_{f_3}
        for ( int i = 1; i < 3; ++i ) {
            for ( int j = 0; j < 2; ++j )
                controlledNot( circ, pair[ i ][ j ], a );
            rotateZ( circ, a, 2 * s * dt );
            for ( int j = 1; j > -1; --j )
                controlledNot( circ, pair[ i ][ j ], a );
        }
    }

    /* ====================================
     *  Measurements
     * ===================================*/
    int outcome;
    cout << "Outcome of the measurement:\n";
    for( int i = 1; i < 4; i++){
        outcome = measure( circ, i );
        cout << "\tq" << i << " = " << outcome << endl;
    }

    /* ====================================
     *  Calculation
     * ===================================*/
    double ExpectValue[] = { 0, 0, 0 };
    int targets[][ 1 ] = { { 1 }, { 2 }, { 3 } };
    enum pauliOpType pauliZ[] = {PAULI_Z};
    enum pauliOpType pauliI[] = {PAULI_I};

    cout << "Solutions:\n";
    for ( int i = 0; i < 3; ++i ) {
        ExpectValue[ i ] += 0.5 * calcExpecPauliProd( circ, targets[ i ], pauliZ, 1, workspace );
        ExpectValue[ i ] += 0.5 * calcExpecPauliProd( circ, targets[ i ], pauliI, 1, workspace );
        cout << "\tx" << targets[ i ][ 0 ]  << " = " << ExpectValue[ i ] << endl;
    }

    /* ====================================
     * Environment destroyed
     * ===================================*/
    destroyQureg( circ, env );
    destroyQureg( workspace, env );
    destroyQuESTEnv( env );
    return 0;
}