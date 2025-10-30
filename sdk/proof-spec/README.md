# @overlay/proof-spec

TypeScript SDK for interacting with the ProofOfAction smart contract with phase support.

## Overview

The `@overlay/proof-spec` SDK provides a simple and type-safe interface for interacting with the ProofOfAction smart contract. It supports attesting actions with phase information (learn/ship/rest) and listening to attestation events.

## Installation

```bash
npm install @overlay/proof-spec
```

## Features

- **Type-Safe**: Full TypeScript support with complete type definitions
- **Phase Support**: Built-in support for LEARN, SHIP, and REST phases
- **Event Listening**: Subscribe to attestation events in real-time
- **Ethers.js Integration**: Built on top of ethers.js v6
- **Simple API**: Clean and intuitive interface

## Usage

### Creating a Client

```typescript
import { createProofSpecClient, Phase } from '@overlay/proof-spec';
import { ethers } from 'ethers';

// With a signer (for transactions)
const provider = new ethers.JsonRpcProvider('https://your-rpc-url');
const signer = await provider.getSigner();
const client = createProofSpecClient(contractAddress, signer);

// With a provider only (for read-only operations)
const readOnlyClient = createProofSpecClient(contractAddress, provider);
```

### Attesting Actions

```typescript
// Attest an action with LEARN phase
const result = await client.attest({
  action: 'Completed TypeScript tutorial',
  phase: Phase.LEARN
});

console.log('Transaction:', result.transactionHash);
console.log('Actor:', result.actor);
console.log('Phase:', result.phase);
console.log('Timestamp:', result.timestamp);

// Attest with SHIP phase
await client.attest({
  action: 'Deployed new feature to production',
  phase: Phase.SHIP
});

// Attest with REST phase
await client.attest({
  action: 'Taking a well-deserved break',
  phase: Phase.REST
});
```

### Getting Attestation Count

```typescript
const count = await client.getAttestationCount('0x1234...');
console.log('Total attestations:', count);
```

### Listening to Events

```typescript
// Subscribe to attestation events
const unsubscribe = client.onAttested((event) => {
  console.log('New attestation:', {
    actor: event.actor,
    action: event.action,
    phase: event.phase,
    timestamp: event.timestamp,
    txHash: event.transactionHash
  });
});

// Stop listening
unsubscribe();
```

## API Reference

### Phase Enum

```typescript
enum Phase {
  LEARN = 0,  // Learning phase
  SHIP = 1,   // Shipping/Building phase
  REST = 2    // Resting phase
}
```

### ProofSpecClient

#### constructor(contractAddress: string, signerOrProvider: Signer | Provider)

Creates a new client instance.

**Parameters:**
- `contractAddress`: Address of the deployed ProofOfAction contract
- `signerOrProvider`: Ethers.js Signer (for transactions) or Provider (read-only)

#### attest(params: AttestParams): Promise<AttestResult>

Attests an action with a specific phase.

**Parameters:**
- `params.action`: Description of the action (non-empty string)
- `params.phase`: Phase enum value (LEARN, SHIP, or REST)

**Returns:** Promise resolving to `AttestResult`

**Throws:**
- `Error` if action is empty
- `Error` if phase is invalid
- `Error` if no signer is provided

#### getAttestationCount(address: string): Promise<bigint>

Gets the total attestation count for an address.

**Parameters:**
- `address`: Ethereum address to query

**Returns:** Promise resolving to the attestation count

#### onAttested(callback: (event: AttestResult) => void): () => void

Subscribes to Attested events.

**Parameters:**
- `callback`: Function to handle new attestation events

**Returns:** Unsubscribe function

### Types

#### AttestParams

```typescript
interface AttestParams {
  action: string;
  phase: Phase;
}
```

#### AttestResult

```typescript
interface AttestResult {
  transactionHash: string;
  actor: string;
  action: string;
  phase: Phase;
  timestamp: bigint;
}
```

## Examples

### Complete Example

```typescript
import { createProofSpecClient, Phase } from '@overlay/proof-spec';
import { ethers } from 'ethers';

async function main() {
  // Setup
  const provider = new ethers.JsonRpcProvider('https://your-rpc-url');
  const signer = await provider.getSigner();
  const contractAddress = '0x...';
  
  // Create client
  const client = createProofSpecClient(contractAddress, signer);
  
  // Subscribe to events
  const unsubscribe = client.onAttested((event) => {
    console.log(`${event.actor} attested: ${event.action} (Phase: ${event.phase})`);
  });
  
  // Attest actions for each phase
  await client.attest({
    action: 'Learned about smart contracts',
    phase: Phase.LEARN
  });
  
  await client.attest({
    action: 'Built and deployed dApp',
    phase: Phase.SHIP
  });
  
  await client.attest({
    action: 'Took a coding break',
    phase: Phase.REST
  });
  
  // Check attestation count
  const signerAddress = await signer.getAddress();
  const count = await client.getAttestationCount(signerAddress);
  console.log(`Total attestations: ${count}`);
  
  // Cleanup
  unsubscribe();
}

main();
```

### Error Handling

```typescript
try {
  await client.attest({
    action: '',  // Empty action
    phase: Phase.LEARN
  });
} catch (error) {
  console.error('Attestation failed:', error.message);
  // Output: "Action cannot be empty"
}

try {
  await client.attest({
    action: 'Test',
    phase: 999 as Phase  // Invalid phase
  });
} catch (error) {
  console.error('Attestation failed:', error.message);
  // Output: "Invalid phase value"
}
```

## Development

### Build

```bash
npm run build
```

### Test

```bash
npm test
```

### Clean

```bash
npm run clean
```

## Version History

### 2.0.0
- Added phase support to attest() method
- Enhanced event emission with phase information
- Updated types and interfaces for phase tracking
- Improved error handling and validation

### 1.0.0
- Initial release

## License

MIT

## Support

For issues and questions, please open an issue on GitHub.
