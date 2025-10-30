# ProofOfAction Smart Contract and SDK Integration

This document provides an overview of the ProofOfAction smart contract and TypeScript SDK integration with phase tracking support.

## Overview

The ProofOfAction system allows users to attest their actions on-chain while tracking the phase they are in:
- **LEARN**: Learning and education activities
- **SHIP**: Building and shipping features
- **REST**: Taking breaks and rest periods

## Components

### 1. Smart Contract (`/contracts`)

**Location**: `/contracts/contracts/ProofOfAction.sol`

The ProofOfAction smart contract is implemented in Solidity 0.8.20 and provides:
- Phase tracking with three phases: LEARN (0), SHIP (1), REST (2)
- Event emission including phase information
- Attestation counting per address
- Input validation

**Key Features**:
- ✅ Emits `Attested` events with phase parameter
- ✅ Validates that actions are not empty
- ✅ Tracks attestation count per address
- ✅ Gas-optimized implementation

### 2. TypeScript SDK (`/sdk/proof-spec`)

**Location**: `/sdk/proof-spec`
**Package**: `@overlay/proof-spec` v2.0.0

The TypeScript SDK provides a type-safe interface for interacting with the ProofOfAction contract:
- Full TypeScript support with type definitions
- Phase enum for type safety
- Event listening capabilities
- Built on ethers.js v6

**Key Features**:
- ✅ `attest()` method accepts phase parameter
- ✅ Propagates phase information to the contract
- ✅ Returns attestation results with phase
- ✅ Event listening with phase information

## Directory Structure

```
/
├── contracts/                    # Smart Contract
│   ├── contracts/
│   │   └── ProofOfAction.sol    # Main contract
│   ├── test/
│   │   └── ProofOfAction.test.js # Contract tests
│   ├── build/                    # Compiled artifacts (gitignored)
│   ├── package.json
│   ├── hardhat.config.js
│   ├── .gitignore
│   └── README.md
│
└── sdk/                          # TypeScript SDK
    └── proof-spec/
        ├── src/
        │   └── index.ts          # SDK implementation
        ├── test/
        │   └── index.test.ts     # SDK tests
        ├── dist/                 # Build output (gitignored)
        ├── package.json
        ├── tsconfig.json
        ├── jest.config.js
        ├── .gitignore
        └── README.md
```

## Usage Examples

### Smart Contract Usage

```solidity
// Deploy the contract
ProofOfAction proofOfAction = new ProofOfAction();

// Attest with LEARN phase
proofOfAction.attest("Completed Solidity tutorial", ProofOfAction.Phase.LEARN);

// Attest with SHIP phase
proofOfAction.attest("Deployed smart contract", ProofOfAction.Phase.SHIP);

// Attest with REST phase
proofOfAction.attest("Taking a break", ProofOfAction.Phase.REST);

// Get attestation count
uint256 count = proofOfAction.getAttestationCount(msg.sender);
```

### SDK Usage

```typescript
import { createProofSpecClient, Phase } from '@overlay/proof-spec';
import { ethers } from 'ethers';

// Setup
const provider = new ethers.JsonRpcProvider('https://your-rpc-url');
const signer = await provider.getSigner();
const contractAddress = '0x...';

// Create client
const client = createProofSpecClient(contractAddress, signer);

// Attest with LEARN phase
const result = await client.attest({
  action: 'Completed TypeScript tutorial',
  phase: Phase.LEARN
});

console.log('Transaction:', result.transactionHash);
console.log('Phase:', result.phase); // 0 (LEARN)

// Attest with SHIP phase
await client.attest({
  action: 'Deployed new feature',
  phase: Phase.SHIP
});

// Attest with REST phase
await client.attest({
  action: 'Taking a coding break',
  phase: Phase.REST
});

// Listen to events
const unsubscribe = client.onAttested((event) => {
  console.log('New attestation:', {
    actor: event.actor,
    action: event.action,
    phase: event.phase, // Phase is included in events
    timestamp: event.timestamp
  });
});
```

## Phase Enum Values

| Phase | Value | Description |
|-------|-------|-------------|
| LEARN | 0 | Learning, education, research activities |
| SHIP | 1 | Building, shipping, deployment activities |
| REST | 2 | Rest, breaks, recovery activities |

## Event Structure

The `Attested` event emitted by the contract includes:

```solidity
event Attested(
    address indexed actor,  // Address that made the attestation
    string action,          // Description of the action
    Phase phase,            // Phase during attestation (0, 1, or 2)
    uint256 timestamp       // Block timestamp
);
```

## Testing

### Contract Tests

The contract includes comprehensive tests in `/contracts/test/ProofOfAction.test.js`. However, due to network restrictions, the full Hardhat test suite cannot be run in this environment.

To run tests in a local environment:
```bash
cd contracts
npm install
npm test
```

### SDK Tests

The SDK includes 11 tests covering all functionality:

```bash
cd sdk/proof-spec
npm install
npm test
```

**Test Results**:
- ✅ All 11 tests passing
- ✅ Constructor tests
- ✅ Attest with all three phases
- ✅ Error handling
- ✅ Event listening
- ✅ Attestation count retrieval

## Building

### Build Contract

```bash
cd contracts
npm run compile
```

Output: Compiled contract ABI and bytecode in `/contracts/build/`

### Build SDK

```bash
cd sdk/proof-spec
npm run build
```

Output: TypeScript declarations and JavaScript files in `/sdk/proof-spec/dist/`

## Version Information

- **Smart Contract**: Solidity 0.8.20
- **SDK Version**: 2.0.0
- **Package Name**: @overlay/proof-spec
- **Ethers.js**: v6.9.0

## Key Changes from Previous Version

### Smart Contract Changes
- ✅ Added `Phase` enum (LEARN, SHIP, REST)
- ✅ Updated `attest()` function to accept phase parameter
- ✅ Updated `Attested` event to include phase parameter
- ✅ All attestations now emit phase information

### SDK Changes (v1.0.0 → v2.0.0)
- ✅ Added `Phase` enum export
- ✅ Updated `AttestParams` interface to include phase
- ✅ Updated `AttestResult` interface to include phase
- ✅ Modified `attest()` method to accept and propagate phase
- ✅ Enhanced event listening to include phase information
- ✅ Added phase validation

## Security Considerations

### Smart Contract
- Input validation for empty actions
- No external calls (no reentrancy risk)
- No ownership or access control (public attestation)
- Immutable attestations (cannot be deleted)

### SDK
- Input validation for action and phase
- Type safety with TypeScript
- Error handling for all operations
- No sensitive data exposure

## Documentation

Comprehensive documentation is available:
- **Contract README**: `/contracts/README.md`
- **SDK README**: `/sdk/proof-spec/README.md`
- **This Integration Guide**: `/INTEGRATION.md`

## Deployment Notes

1. **Contract Deployment**:
   - Deploy ProofOfAction.sol to desired network
   - Note the contract address
   - Verify the contract on block explorer

2. **SDK Integration**:
   - Install: `npm install @overlay/proof-spec`
   - Import: `import { createProofSpecClient, Phase } from '@overlay/proof-spec'`
   - Initialize with contract address and signer/provider

3. **Testing**:
   - Test contract on testnet first
   - Verify event emission includes phase
   - Test SDK integration end-to-end

## Support

For issues or questions:
- Smart Contract: See `/contracts/README.md`
- SDK: See `/sdk/proof-spec/README.md`
- Integration: This document

## License

MIT License - See individual package.json files for details.
